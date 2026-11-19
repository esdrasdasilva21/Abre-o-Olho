
using System;
using System.Threading;
using System.Threading.Tasks;
using Abre_o_Olho.App.Services;

namespace Abre_o_Olho.App.Services
{
    public class HidReaderService : IDisposable
    {
        private readonly HidService _hidService;

        private CancellationTokenSource? _cancellation;
        private Task? _readingTask;

        public event Action<byte[]>? ReportReceived;
        public event Action<Exception>? ReadingError;

        public bool IsReading { get; private set; }

        public HidReaderService(HidService hidService)
        {
            _hidService = hidService;
        }

        public void StartReading()
        {
            if (IsReading)
                return;

            _cancellation = new CancellationTokenSource();

            CancellationToken token = _cancellation.Token;

            IsReading = true;

            _readingTask = Task.Run(
                () => ReadLoop(token),
                token
            );
        }

        private void ReadLoop(CancellationToken token)
        {
            try
            {
                while (!token.IsCancellationRequested)
                {
                    try
                    {
                        byte[] report = _hidService.ReadReport();

                        if (report.Length > 0)
                        {
                            ReportReceived?.Invoke(report);
                        }
                    }
                    catch (TimeoutException)
                    {
                        // Aguarda o próximo relatório.
                    }
                }
            }
            catch (Exception ex)
            {
                if (!token.IsCancellationRequested)
                {
                    ReadingError?.Invoke(ex);
                }
            }
            finally
            {
                IsReading = false;
            }
        }

        public async Task StopReadingAsync()
        {
            if (_cancellation == null)
                return;

            _cancellation.Cancel();

            if (_readingTask != null)
            {
                await _readingTask;
            }

            _cancellation.Dispose();
            _cancellation = null;
            _readingTask = null;

            IsReading = false;
        }

        public void Dispose()
        {
            StopReadingAsync().GetAwaiter().GetResult();
        }
    }
}


using System;
using System.Threading;
using System.Threading.Tasks;
using Abre_o_Olho.App.Models;

namespace Abre_o_Olho.App.Services
{
    public class HidReaderService : IDisposable
    {
        private readonly HidService _hidService;
        private readonly HidDataParser _dataParser;

        private CancellationTokenSource? _cancellation;
        private Task? _readingTask;

        public event Action<HidReport>? ReportReceived;

        public event Action<SensorData>? SensorDataReceived;

        public event Action<Exception>? ReadingError;

        public bool IsReading { get; private set; }

        public HidReaderService(HidService hidService)
        {
            _hidService = hidService;
            _dataParser = new HidDataParser();
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
                        byte[] data = _hidService.ReadReport();

                        if (data.Length == 0)
                            continue;

                        HidReport report = new HidReport
                        {
                            ReportId = data[0],
                            RawData = data,
                            ReceivedAt = DateTime.Now
                        };

                        // Disponibiliza o relatório bruto.
                        ReportReceived?.Invoke(report);

                        // Interpreta os dados dos sensores.
                        if (_dataParser.TryParse(
                            report,
                            out SensorData? sensorData))
                        {
                            if (sensorData != null)
                            {
                                SensorDataReceived?.Invoke(sensorData);
                            }
                        }
                    }
                    catch (TimeoutException)
                    {
                        // Continua a aguardar novos relatórios.
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

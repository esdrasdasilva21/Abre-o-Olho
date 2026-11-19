
using System;
using System.Linq;
using HidSharp;

namespace Abre_o_Olho.App.Services
{
    public class HidService
    {
        private const int VendorId = 0x6021;
        private const int ProductId = 0x0001;

        private readonly HidDeviceLoader _deviceLoader;

        private HidDevice? _device;
        private HidStream? _stream;

        public HidService()
        {
            _deviceLoader = new HidDeviceLoader();
        }

        public HidDevice? FindDevice()
        {
            return _deviceLoader
                .GetDevices(VendorId, ProductId)
                .FirstOrDefault();
        }

        public bool IsDeviceConnected()
        {
            return FindDevice() != null;
        }

        public bool OpenConnection()
        {
            try
            {
                _device = FindDevice();

                if (_device == null)
                    return false;

                if (!_device.TryOpen(out _stream))
                    return false;

                _stream.ReadTimeout = 1000;

                return true;
            }
            catch
            {
                CloseConnection();
                return false;
            }
        }

        public byte[] ReadReport()
        {
            if (_stream == null)
                throw new InvalidOperationException(
                    "O dispositivo não está ligado."
                );

            byte[] buffer = new byte[64];

            int bytesRead = _stream.Read(
                buffer,
                0,
                buffer.Length
            );

            return buffer.Take(bytesRead).ToArray();
        }

        public void CloseConnection()
        {
            _stream?.Close();
            _stream?.Dispose();

            _stream = null;
            _device = null;
        }
    }
}

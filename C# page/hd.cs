
using HidSharp;
using System.Collections.Generic;
using System.Linq;

namespace Abre_o_Olho.App.Services
{
    public class HidService
    {
        private const int VendorId = 0x6021;
        private const int ProductId = 0x0001;

        private readonly HidDeviceLoader _deviceLoader;

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

        public IEnumerable<HidDevice> GetDevices()
        {
            return _deviceLoader.GetDevices(VendorId, ProductId);
        }
    }
}

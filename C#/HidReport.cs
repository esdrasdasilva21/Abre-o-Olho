
using System;

namespace Abre_o_Olho.App.Models
{
    public class HidReport
    {
        public byte ReportId { get; set; }

        public byte[] RawData { get; set; } = Array.Empty<byte>();

        public int Length => RawData.Length;

        public DateTime ReceivedAt { get; set; } = DateTime.Now;

        public string GetHexData()
        {
            return BitConverter.ToString(RawData);
        }
    }
}

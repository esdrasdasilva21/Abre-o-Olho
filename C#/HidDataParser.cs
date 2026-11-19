
using System;
using Abre_o_Olho.App.Models;

namespace Abre_o_Olho.App.Services
{
    public class HidDataParser
    {
        private const byte TelemetryReportId = 0x11;
        private const int ExpectedReportLength = 40;

        public bool IsTelemetryReport(HidReport report)
        {
            return report != null
                && report.ReportId == TelemetryReportId;
        }

        public bool TryParse(
            HidReport report,
            out SensorData? sensorData)
        {
            sensorData = null;

            if (!IsTelemetryReport(report))
                return false;

            byte[] data = report.RawData;

            if (data.Length != ExpectedReportLength)
                return false;

            try
            {
                sensorData = new SensorData
                {
                    Counter = BitConverter.ToUInt16(data, 1),

                    Temperature = unchecked(
                        (sbyte)data[3]
                    ),

                    Gyroscope = new int[]
                    {
                        BitConverter.ToInt32(data, 4),
                        BitConverter.ToInt32(data, 8),
                        BitConverter.ToInt32(data, 12)
                    },

                    Accelerometer = new int[]
                    {
                        BitConverter.ToInt32(data, 16),
                        BitConverter.ToInt32(data, 20),
                        BitConverter.ToInt32(data, 24)
                    },

                    Magnetometer = new int[]
                    {
                        BitConverter.ToInt32(data, 28),
                        BitConverter.ToInt32(data, 32),
                        BitConverter.ToInt32(data, 36)
                    },

                    ReceivedAt = report.ReceivedAt
                };

                return true;
            }
            catch
            {
                sensorData = null;
                return false;
            }
        }
    }
}

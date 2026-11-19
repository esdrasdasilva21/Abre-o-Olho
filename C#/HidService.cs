
using System;
using System.Buffers.Binary;
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

            if (report == null ||
                !IsTelemetryReport(report) ||
                report.RawData.Length != ExpectedReportLength)
            {
                return false;
            }

            byte[] data = report.RawData;

            ushort counter =
                BinaryPrimitives.ReadUInt16LittleEndian(
                    data.AsSpan(1, 2)
                );

            sbyte temperature = unchecked((sbyte)data[3]);

            int[] gyroscope = ReadSensorValues(data, 4);
            int[] accelerometer = ReadSensorValues(data, 16);
            int[] magnetometer = ReadSensorValues(data, 28);

            sensorData = new SensorData
            {
                Counter = counter,
                Temperature = temperature,
                Gyroscope = gyroscope,
                Accelerometer = accelerometer,
                Magnetometer = magnetometer,
                ReceivedAt = report.ReceivedAt
            };

            return true;
        }

        private int[] ReadSensorValues(byte[] data, int offset)
        {
            int[] values = new int[3];

            for (int i = 0; i < 3; i++)
            {
                values[i] =
                    BinaryPrimitives.ReadInt32LittleEndian(
                        data.AsSpan(offset + (i * 4), 4)
                    );
            }

            return values;
        }
    }
}

using System;
using System.Buffers.Binary;
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

            if (report == null ||
                !IsTelemetryReport(report) ||
                report.RawData.Length != ExpectedReportLength)
            {
                return false;
            }

            byte[] data = report.RawData;

            ushort counter =
                BinaryPrimitives.ReadUInt16LittleEndian(
                    data.AsSpan(1, 2)
                );

            sbyte temperature = unchecked((sbyte)data[3]);

            int[] gyroscope = ReadSensorValues(data, 4);
            int[] accelerometer = ReadSensorValues(data, 16);
            int[] magnetometer = ReadSensorValues(data, 28);

            sensorData = new SensorData
            {
                Counter = counter,
                Temperature = temperature,
                Gyroscope = gyroscope,
                Accelerometer = accelerometer,
                Magnetometer = magnetometer,
                ReceivedAt = report.ReceivedAt
            };

            return true;
        }

        private int[] ReadSensorValues(byte[] data, int offset)
        {
            int[] values = new int[3];

            for (int i = 0; i < 3; i++)
            {
                values[i] =
                    BinaryPrimitives.ReadInt32LittleEndian(
                        data.AsSpan(offset + (i * 4), 4)
                    );
            }

            return values;
        }
    }
}

using System;
using System.Buffers.Binary;
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

            if (report == null ||
                !IsTelemetryReport(report) ||
                report.RawData.Length != ExpectedReportLength)
            {
                return false;
            }

            byte[] data = report.RawData;

            ushort counter =
                BinaryPrimitives.ReadUInt16LittleEndian(
                    data.AsSpan(1, 2)
                );

            sbyte temperature = unchecked((sbyte)data[3]);

            int[] gyroscope = ReadSensorValues(data, 4);
            int[] accelerometer = ReadSensorValues(data, 16);
            int[] magnetometer = ReadSensorValues(data, 28);

            sensorData = new SensorData
            {
                Counter = counter,
                Temperature = temperature,
                Gyroscope = gyroscope,
                Accelerometer = accelerometer,
                Magnetometer = magnetometer,
                ReceivedAt = report.ReceivedAt
            };

            return true;
        }

        private int[] ReadSensorValues(byte[] data, int offset)
        {
            int[] values = new int[3];

            for (int i = 0; i < 3; i++)
            {
                values[i] =
                    BinaryPrimitives.ReadInt32LittleEndian(
                        data.AsSpan(offset + (i * 4), 4)
                    );
            }

            return values;
        }
    }
}

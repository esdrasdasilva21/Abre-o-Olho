
using System;

namespace Abre_o_Olho.App.Models
{
    public class SensorData
    {
        public ushort Counter { get; set; }

        public sbyte Temperature { get; set; }

        public int[] Gyroscope { get; set; } = new int[3];

        public int[] Accelerometer { get; set; } = new int[3];

        public int[] Magnetometer { get; set; } = new int[3];

        public DateTime ReceivedAt { get; set; } = DateTime.Now;
    }
}

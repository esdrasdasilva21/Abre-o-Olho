
import Chart from 'chart.js/auto';

let gyroscopeChart: Chart | null = null;
let accelerometerChart: Chart | null = null;
let magnetometerChart: Chart | null = null;

const MAX_POINTS = 20;

function createChart(
  canvasId: string,
  title: string
): Chart {
  const canvas = document.getElementById(canvasId) as HTMLCanvasElement;

  return new Chart(canvas, {
    type: 'line',

    data: {
      labels: [],

      datasets: [
        {
          label: 'Eixo X',
          data: [],
          borderColor: '#38bdf8',
          backgroundColor: '#38bdf8',
          tension: 0.3
        },
        {
          label: 'Eixo Y',
          data: [],
          borderColor: '#a78bfa',
          backgroundColor: '#a78bfa',
          tension: 0.3
        },
        {
          label: 'Eixo Z',
          data: [],
          borderColor: '#34d399',
          backgroundColor: '#34d399',
          tension: 0.3
        }
      ]
    },

    options: {
      responsive: true,
      maintainAspectRatio: false,
      animation: false,

      plugins: {
        title: {
          display: true,
          text: title,
          color: '#ffffff'
        },

        legend: {
          labels: {
            color: '#d1d5db'
          }
        }
      },

      scales: {
        x: {
          ticks: {
            color: '#9ca3af'
          },
          grid: {
            color: '#374151'
          }
        },

        y: {
          ticks: {
            color: '#9ca3af'
          },
          grid: {
            color: '#374151'
          }
        }
      }
    }
  });
}

export function initializeSensorCharts(): void {
  gyroscopeChart = createChart(
    'gyroscope-chart',
    'Giroscópio'
  );

  accelerometerChart = createChart(
    'accelerometer-chart',
    'Acelerómetro'
  );

  magnetometerChart = createChart(
    'magnetometer-chart',
    'Magnetómetro'
  );
}

function addChartData(
  chart: Chart | null,
  values: number[]
): void {
  if (!chart) return;

  const timestamp = new Date().toLocaleTimeString();

  chart.data.labels?.push(timestamp);

  chart.data.datasets.forEach((dataset, index) => {
    dataset.data.push(values[index] ?? 0);
  });

  if (chart.data.labels && chart.data.labels.length > MAX_POINTS) {
    chart.data.labels.shift();

    chart.data.datasets.forEach(dataset => {
      dataset.data.shift();
    });
  }

  chart.update('none');
}

export function updateSensorCharts(
  gyroscope: number[],
  accelerometer: number[],
  magnetometer: number[]
): void {
  addChartData(gyroscopeChart, gyroscope);
  addChartData(accelerometerChart, accelerometer);
  addChartData(magnetometerChart, magnetometer);
}

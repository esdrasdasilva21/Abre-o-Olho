import './style.css';
import {
  checkApiConnection,
  getLatestTelemetry
} from './services/TelemetryService';


async function updateApiStatus(): Promise<void> {
  const statusElement = document.querySelector<HTMLElement>('#api-status');
  const connectionElement = document.querySelector<HTMLElement>('.connection');

  if (!statusElement || !connectionElement) {
    return;
  }

  const connected = await checkApiConnection();

  if (connected) {
    statusElement.textContent = 'Online';
    connectionElement.innerHTML = `
      <span class="status-dot"></span>
      <span>API CONECTADA</span>
    `;
    connectionElement.classList.add('connected');
  } else {
    statusElement.textContent = 'Offline';
    connectionElement.innerHTML = `
      <span class="status-dot"></span>
      <span>API DESCONECTADA</span>
    `;
    connectionElement.classList.remove('connected');
  }
}

updateApiStatus();

setInterval(updateApiStatus, 5000);



async function updateSensorData(): Promise<void> {
  try {
    const data = await getLatestTelemetry();

    if (!data) {
      console.log('Ainda não existem dados de telemetria.');
      return;
    }

    document.querySelector('#counter')!.textContent =
      String(data.counter);

    document.querySelector('#temperature')!.textContent =
      `${data.temperature} °C`;

    document.querySelector('#gyro-x')!.textContent =
      String(data.gyroscope[0]);

    document.querySelector('#gyro-y')!.textContent =
      String(data.gyroscope[1]);

    document.querySelector('#gyro-z')!.textContent =
      String(data.gyroscope[2]);

    document.querySelector('#acc-x')!.textContent =
      String(data.accelerometer[0]);

    document.querySelector('#acc-y')!.textContent =
      String(data.accelerometer[1]);

    document.querySelector('#acc-z')!.textContent =
      String(data.accelerometer[2]);

    document.querySelector('#mag-x')!.textContent =
      String(data.magnetometer[0]);

    document.querySelector('#mag-y')!.textContent =
      String(data.magnetometer[1]);

    document.querySelector('#mag-z')!.textContent =
      String(data.magnetometer[2]);

  } catch (error) {
    console.error('Erro ao atualizar os sensores:', error);
  }
}

updateSensorData();

setInterval(updateSensorData, 1000);

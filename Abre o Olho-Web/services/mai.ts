import './style.css';
import { checkApiConnection } from './services/TelemetryService';


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

export interface SensorTelemetry {
  counter: number;
  temperature: number;
  gyroscope: number[];
  accelerometer: number[];
  magnetometer: number[];
  receivedAt: string;
}

// Substitui pela porta real da tua API.
const API_URL = 'http://localhost:5000';

export async function getLatestTelemetry(): Promise<SensorTelemetry | null> {
  const response = await fetch(`${API_URL}/api/telemetry/latest`);

  if (response.status === 404) {
    return null;
  }

  if (!response.ok) {
    throw new Error('Não foi possível obter a telemetria.');
  }

  return await response.json() as SensorTelemetry;
}

export async function checkApiConnection(): Promise<boolean> {
  try {
    const response = await fetch(`${API_URL}/`);
    return response.ok;
  } catch {
    return false;
  }
}


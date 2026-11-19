
export interface SensorTelemetry {
  counter: number;
  temperature: number;
  gyroscope: number[];
  accelerometer: number[];
  magnetometer: number[];
  receivedAt: string;
}

const API_URL = 'http://localhost:5000';

export async function getLatestTelemetry(): Promise<SensorTelemetry | null> {
  try {
    const response = await fetch(
      `${API_URL}/api/telemetry/latest`
    );

    if (response.status === 404) {
      return null;
    }

    if (!response.ok) {
      throw new Error('Erro ao consultar a telemetria.');
    }

    const data: SensorTelemetry = await response.json();

    return data;
  } catch (error) {
    console.error('Erro de comunicação com a API:', error);
    throw error;
  }
}

export async function checkApiConnection(): Promise<boolean> {
  try {
    const response = await fetch(`${API_URL}/`);

    return response.ok;
  } catch {
    return false;
  }
}

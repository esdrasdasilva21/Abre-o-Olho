
using System;
using System.Net.Http;
using System.Net.Http.Json;
using System.Threading.Tasks;
using Abre_o_Olho.App.Models;

namespace Abre_o_Olho.App.Services
{
    public class TelemetryApiClient : IDisposable
    {
        private readonly HttpClient _httpClient;

        public TelemetryApiClient()
        {
            _httpClient = new HttpClient
            {
                BaseAddress = new Uri("http://localhost:5000"),
                Timeout = TimeSpan.FromSeconds(5)
            };
        }

        public async Task<bool> SendTelemetryAsync(SensorData data)
        {
            try
            {
                HttpResponseMessage response =
                    await _httpClient.PostAsJsonAsync(
                        "/api/telemetry",
                        data
                    );

                return response.IsSuccessStatusCode;
            }
            catch (HttpRequestException)
            {
                return false;
            }
            catch (TaskCanceledException)
            {
                return false;
            }
        }

        public void Dispose()
        {
            _httpClient.Dispose();
        }
    }
}

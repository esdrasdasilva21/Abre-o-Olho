
var builder = WebApplication.CreateBuilder(args);

// Configuração de CORS para permitir
// futuras ligações do dashboard TypeScript.
builder.Services.AddCors(options =>
{
    options.AddPolicy("DashboardPolicy", policy =>
    {
        policy
            .AllowAnyOrigin()
            .AllowAnyHeader()
            .AllowAnyMethod();
    });
});

var app = builder.Build();

app.UseCors("DashboardPolicy");

// Armazenamento temporário da última telemetria.
SensorTelemetry? latestTelemetry = null;

// Endpoint inicial.
app.MapGet("/", () =>
{
    return Results.Ok(new
    {
        application = "Abre o Olho API",
        status = "Online",
        version = "1.0"
    });
});

// Receber dados dos sensores.
app.MapPost("/api/telemetry", (SensorTelemetry data) =>
{
    latestTelemetry = data;

    return Results.Ok(new
    {
        message = "Telemetria recebida com sucesso!",
        receivedAt = DateTime.UtcNow
    });
});

// Consultar a última telemetria.
app.MapGet("/api/telemetry/latest", () =>
{
    if (latestTelemetry == null)
    {
        return Results.NotFound(new
        {
            message = "Ainda não existem dados de telemetria."
        });
    }

    return Results.Ok(latestTelemetry);
});

app.Run();

public class SensorTelemetry
{
    public ushort Counter { get; set; }

    public sbyte Temperature { get; set; }

    public int[] Gyroscope { get; set; } = new int[3];

    public int[] Accelerometer { get; set; } = new int[3];

    public int[] Magnetometer { get; set; } = new int[3];

    public DateTime ReceivedAt { get; set; }
}

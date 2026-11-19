
import './style.css';

document.querySelector<HTMLDivElement>('#app')!.innerHTML = `
  <div class="dashboard">

    <header class="topbar">
      <div class="brand">
        <div class="brand-icon">AO</div>
        <div>
          <h1>ABRE O OLHO</h1>
          <span>Sensor Monitoring System</span>
        </div>
      </div>

      <div class="connection">
        <span class="status-dot"></span>
        <span>API DESCONECTADA</span>
      </div>
    </header>

    <main>
      <section class="welcome">
        <div>
          <p class="eyebrow">CENTRAL DE MONITORIZAÇÃO</p>
          <h2>Dashboard de Telemetria</h2>
          <p class="description">
            Monitorização dos dados recebidos do dispositivo USB.
          </p>
        </div>

        <div class="device-card">
          <span>DISPOSITIVO</span>
          <strong>Open DoVision</strong>
          <small>VID: 0x6021 | PID: 0x0001</small>
        </div>
      </section>

      <section class="metrics">
        <article class="metric-card">
          <span class="metric-label">RELATÓRIOS RECEBIDOS</span>
          <strong id="counter">0</strong>
          <small>Contador de telemetria</small>
        </article>

        <article class="metric-card">
          <span class="metric-label">TEMPERATURA</span>
          <strong id="temperature">-- °C</strong>
          <small>Temperatura do sensor</small>
        </article>

        <article class="metric-card">
          <span class="metric-label">ESTADO DA API</span>
          <strong id="api-status">Offline</strong>
          <small>Servidor local ASP.NET Core</small>
        </article>
      </section>

      <section class="sensors-section">
        <div class="section-heading">
          <div>
            <p class="eyebrow">LEITURA DOS SENSORES</p>
            <h2>Dados dos sensores</h2>
          </div>
          <span class="live-label">LIVE MONITOR</span>
        </div>

        <div class="sensor-grid">

          <article class="sensor-card">
            <div class="sensor-heading">
              <span class="sensor-icon">G</span>
              <h3>Giroscópio</h3>
            </div>

            <div class="axis-values">
              <div><span>X</span><strong id="gyro-x">--</strong></div>
              <div><span>Y</span><strong id="gyro-y">--</strong></div>
              <div><span>Z</span><strong id="gyro-z">--</strong></div>
            </div>
          </article>

          <article class="sensor-card">
            <div class="sensor-heading">
              <span class="sensor-icon">A</span>
              <h3>Acelerómetro</h3>
            </div>

            <div class="axis-values">
              <div><span>X</span><strong id="acc-x">--</strong></div>
              <div><span>Y</span><strong id="acc-y">--</strong></div>
              <div><span>Z</span><strong id="acc-z">--</strong></div>
            </div>
          </article>

          <article class="sensor-card">
            <div class="sensor-heading">
              <span class="sensor-icon">M</span>
              <h3>Magnetómetro</h3>
            </div>

            <div class="axis-values">
              <div><span>X</span><strong id="mag-x">--</strong></div>
              <div><span>Y</span><strong id="mag-y">--</strong></div>
              <div><span>Z</span><strong id="mag-z">--</strong></div>
            </div>
          </article>

        </div>
      </section>

      <footer>
        <span>ABRE O OLHO © 2026</span>
        <span>Multimedia Production Project</span>
      </footer>

    </main>
  </div>
`;


using System;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;
using Abre_o_Olho.App.Models;
using Abre_o_Olho.App.Services;

namespace Abre_o_Olho.App.Forms
{
    public class MainForm : Form
    {
        private readonly HidService _hidService;
        private readonly HidReaderService _hidReader;
        private readonly HidDataParser _dataParser;

        private readonly StringBuilder _reportLog = new();

        private Label _titleLabel;
        private Label _statusLabel;
        private Label _deviceLabel;
        private Label _readingLabel;
        private Label _sensorTitleLabel;
        private Label _rawTitleLabel;

        private Button _scanButton;
        private Button _startButton;
        private Button _stopButton;
        private Button _exportButton;

        private TextBox _sensorBox;
        private TextBox _dataBox;

        private int _reportCount = 0;

        public MainForm()
        {
            _hidService = new HidService();
            _hidReader = new HidReaderService(_hidService);
            _dataParser = new HidDataParser();

            _hidReader.ReportReceived += OnReportReceived;
            _hidReader.ReadingError += OnReadingError;

            InitializeInterface();
        }

        private void InitializeInterface()
        {
            Text = "Abre o Olho | USB Monitor";
            Size = new Size(850, 850);

            StartPosition = FormStartPosition.CenterScreen;
            BackColor = Color.FromArgb(24, 28, 36);

            FormBorderStyle = FormBorderStyle.FixedSingle;
            MaximizeBox = false;

            _titleLabel = new Label
            {
                Text = "ABRE O OLHO",
                Font = new Font("Segoe UI", 22, FontStyle.Bold),
                ForeColor = Color.White,
                AutoSize = true,
                Location = new Point(35, 25)
            };

            _statusLabel = new Label
            {
                Text = "Estado: Aguardando ligação",
                Font = new Font("Segoe UI", 11),
                ForeColor = Color.LightGray,
                AutoSize = true,
                Location = new Point(38, 95)
            };

            _deviceLabel = new Label
            {
                Text = "VID: 0x6021 | PID: 0x0001",
                Font = new Font("Segoe UI", 10),
                ForeColor = Color.Silver,
                AutoSize = true,
                Location = new Point(38, 130)
            };

            _scanButton = new Button
            {
                Text = "LIGAR DISPOSITIVO",
                Size = new Size(190, 42),
                Location = new Point(38, 175),
                BackColor = Color.FromArgb(40, 110, 180),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat
            };

            _startButton = new Button
            {
                Text = "INICIAR LEITURA",
                Size = new Size(190, 42),
                Location = new Point(245, 175),
                BackColor = Color.FromArgb(35, 145, 95),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat,
                Enabled = false
            };

            _stopButton = new Button
            {
                Text = "PARAR LEITURA",
                Size = new Size(190, 42),
                Location = new Point(452, 175),
                BackColor = Color.FromArgb(180, 65, 65),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat,
                Enabled = false
            };

            _exportButton = new Button
            {
                Text = "EXPORTAR",
                Size = new Size(130, 42),
                Location = new Point(659, 175),
                BackColor = Color.FromArgb(100, 85, 160),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat
            };

            _readingLabel = new Label
            {
                Text = "Relatórios recebidos: 0",
                Font = new Font("Segoe UI", 10, FontStyle.Bold),
                ForeColor = Color.LightGreen,
                AutoSize = true,
                Location = new Point(38, 235)
            };

            _sensorTitleLabel = new Label
            {
                Text = "DADOS DOS SENSORES",
                Font = new Font("Segoe UI", 12, FontStyle.Bold),
                ForeColor = Color.White,
                AutoSize = true,
                Location = new Point(38, 270)
            };

            _sensorBox = new TextBox
            {
                Multiline = true,
                ReadOnly = true,
                ScrollBars = ScrollBars.Vertical,
                Font = new Font("Consolas", 11),
                BackColor = Color.FromArgb(13, 17, 23),
                ForeColor = Color.LightGreen,
                BorderStyle = BorderStyle.FixedSingle,
                Location = new Point(38, 305),
                Size = new Size(755, 190),
                Text =
                    "Aguardando dados de telemetria...\r\n\r\n" +
                    "Temperatura: --\r\n" +
                    "Giroscópio: --\r\n" +
                    "Acelerómetro: --\r\n" +
                    "Magnetómetro: --"
            };

            _rawTitleLabel = new Label
            {
                Text = "RELATÓRIOS HID RECEBIDOS",
                Font = new Font("Segoe UI", 12, FontStyle.Bold),
                ForeColor = Color.White,
                AutoSize = true,
                Location = new Point(38, 515)
            };

            _dataBox = new TextBox
            {
                Multiline = true,
                ReadOnly = true,
                ScrollBars = ScrollBars.Vertical,
                Font = new Font("Consolas", 9),
                BackColor = Color.FromArgb(13, 17, 23),
                ForeColor = Color.LightGreen,
                BorderStyle = BorderStyle.FixedSingle,
                Location = new Point(38, 550),
                Size = new Size(755, 230)
            };

            _scanButton.Click += ScanButton_Click;
            _startButton.Click += StartButton_Click;
            _stopButton.Click += StopButton_Click;
            _exportButton.Click += ExportButton_Click;

            Controls.Add(_titleLabel);
            Controls.Add(_statusLabel);
            Controls.Add(_deviceLabel);

            Controls.Add(_scanButton);
            Controls.Add(_startButton);
            Controls.Add(_stopButton);
            Controls.Add(_exportButton);

            Controls.Add(_readingLabel);
            Controls.Add(_sensorTitleLabel);
            Controls.Add(_sensorBox);

            Controls.Add(_rawTitleLabel);
            Controls.Add(_dataBox);
        }

        private void ScanButton_Click(object? sender, EventArgs e)
        {
            bool connected = _hidService.OpenConnection();

            if (connected)
            {
                _statusLabel.Text = "Estado: Dispositivo ligado!";
                _statusLabel.ForeColor = Color.LightGreen;

                _scanButton.Enabled = false;
                _startButton.Enabled = true;

                MessageBox.Show(
                    "Comunicação HID estabelecida!",
                    "Abre o Olho",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Information
                );
            }
            else
            {
                _statusLabel.Text = "Estado: Falha na ligação.";
                _statusLabel.ForeColor = Color.OrangeRed;

                MessageBox.Show(
                    "Não foi possível ligar ao dispositivo.",
                    "Erro HID",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning
                );
            }
        }

        private void StartButton_Click(object? sender, EventArgs e)
        {
            _hidReader.StartReading();

            _statusLabel.Text = "Estado: A receber relatórios HID...";
            _statusLabel.ForeColor = Color.LightGreen;

            _startButton.Enabled = false;
            _stopButton.Enabled = true;
        }

        private async void StopButton_Click(object? sender, EventArgs e)
        {
            await _hidReader.StopReadingAsync();

            _statusLabel.Text = "Estado: Leitura interrompida.";
            _statusLabel.ForeColor = Color.Orange;

            _startButton.Enabled = true;
            _stopButton.Enabled = false;
        }

        private void OnReportReceived(HidReport report)
        {
            if (IsDisposed || !IsHandleCreated)
                return;

            BeginInvoke(new Action(() =>
            {
                _reportCount++;

                _readingLabel.Text =
                    $"Relatórios recebidos: {_reportCount}";

                string hexData = report.GetHexData();

                string logEntry =
                    $"[{report.ReceivedAt:yyyy-MM-dd HH:mm:ss.fff}] " +
                    $"ID: 0x{report.ReportId:X2} | " +
                    $"Tamanho: {report.Length} bytes\r\n" +
                    hexData + "\r\n";

                if (_dataParser.TryParse(report, out SensorData? sensorData)
                    && sensorData != null)
                {
                    string sensorText = FormatSensorData(sensorData);

                    _sensorBox.Text = sensorText;

                    logEntry +=
                        "\r\n--- DADOS INTERPRETADOS ---\r\n" +
                        sensorText + "\r\n";
                }
                else
                {
                    logEntry +=
                        "Relatório recebido, mas não foi interpretado " +
                        "como telemetria válida.\r\n";
                }

                logEntry += "\r\n";

                _reportLog.Append(logEntry);
                _dataBox.AppendText(logEntry);
            }));
        }

        private string FormatSensorData(SensorData data)
        {
            return
                $"Contador: {data.Counter}\r\n" +
                $"Temperatura: {data.Temperature} °C\r\n\r\n" +

                "GIROSCÓPIO\r\n" +
                $"X: {data.Gyroscope[0]} | " +
                $"Y: {data.Gyroscope[1]} | " +
                $"Z: {data.Gyroscope[2]}\r\n\r\n" +

                "ACELERÓMETRO\r\n" +
                $"X: {data.Accelerometer[0]} | " +
                $"Y: {data.Accelerometer[1]} | " +
                $"Z: {data.Accelerometer[2]}\r\n\r\n" +

                "MAGNETÓMETRO\r\n" +
                $"X: {data.Magnetometer[0]} | " +
                $"Y: {data.Magnetometer[1]} | " +
                $"Z: {data.Magnetometer[2]}";
        }

        private void ExportButton_Click(object? sender, EventArgs e)
        {
            if (_reportLog.Length == 0)
            {
                MessageBox.Show(
                    "Ainda não existem relatórios para exportar. " +
                    "Inicia primeiro a leitura HID.",
                    "Sem dados",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning
                );

                return;
            }

            using (SaveFileDialog dialog = new SaveFileDialog())
            {
                dialog.Title = "Exportar relatórios HID";
                dialog.Filter = "Ficheiros de texto (*.txt)|*.txt";
                dialog.FileName = "Abre-o-Olho-Relatorios.txt";
                dialog.DefaultExt = "txt";

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        File.WriteAllText(
                            dialog.FileName,
                            _reportLog.ToString(),
                            Encoding.UTF8
                        );

                        MessageBox.Show(
                            "Relatórios exportados com sucesso!",
                            "Exportação concluída",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Information
                        );
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(
                            ex.Message,
                            "Erro ao exportar",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error
                        );
                    }
                }
            }
        }

        private void OnReadingError(Exception ex)
        {
            if (IsDisposed || !IsHandleCreated)
                return;

            BeginInvoke(new Action(() =>
            {
                _statusLabel.Text = "Estado: Erro na leitura HID.";
                _statusLabel.ForeColor = Color.OrangeRed;

                _dataBox.AppendText(
                    "ERRO: " + ex.Message + "\r\n"
                );

                _startButton.Enabled = false;
                _stopButton.Enabled = false;
            }));
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            _hidReader.Dispose();
            _hidService.CloseConnection();

            base.OnFormClosing(e);
        }
    }
}

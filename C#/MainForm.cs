
using System;
using System.Drawing;
using System.Windows.Forms;
using Abre_o_Olho.App.Services;

namespace Abre_o_Olho.App.Forms
{
    public class MainForm : Form
    {
        private readonly HidService _hidService;

        private Label _titleLabel;
        private Label _statusLabel;
        private Label _deviceLabel;
        private Button _scanButton;

        public MainForm()
        {
            _hidService = new HidService();

            InitializeInterface();
        }

        private void InitializeInterface()
        {
            Text = "Abre o Olho | USB Monitor";
            Size = new Size(650, 400);
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
                Location = new Point(35, 35)
            };

            _statusLabel = new Label
            {
                Text = "Estado: Aguardando verificação",
                Font = new Font("Segoe UI", 12),
                ForeColor = Color.LightGray,
                AutoSize = true,
                Location = new Point(38, 125)
            };

            _deviceLabel = new Label
            {
                Text = "VID: 0x6021 | PID: 0x0001",
                Font = new Font("Segoe UI", 10),
                ForeColor = Color.Silver,
                AutoSize = true,
                Location = new Point(38, 165)
            };

            _scanButton = new Button
            {
                Text = "PROCURAR DISPOSITIVO",
                Size = new Size(240, 45),
                Location = new Point(38, 230),
                BackColor = Color.FromArgb(40, 110, 180),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat,
                Cursor = Cursors.Hand
            };

            _scanButton.FlatAppearance.BorderSize = 0;

            // Associar o evento de clique
            _scanButton.Click += ScanButton_Click;

            Controls.Add(_titleLabel);
            Controls.Add(_statusLabel);
            Controls.Add(_deviceLabel);
            Controls.Add(_scanButton);
        }

        private void ScanButton_Click(object? sender, EventArgs e)
        {
            try
            {
                // Teste inicial do evento
                _statusLabel.Text = "Estado: A verificar...";
                _statusLabel.ForeColor = Color.Yellow;

                // Procurar dispositivo USB HID
                bool connected = _hidService.IsDeviceConnected();

                if (connected)
                {
                    _statusLabel.Text = "Estado: Dispositivo encontrado!";
                    _statusLabel.ForeColor = Color.LightGreen;

                    MessageBox.Show(
                        "O dispositivo Abre o Olho foi encontrado!",
                        "USB HID",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Information
                    );
                }
                else
                {
                    _statusLabel.Text = "Estado: Dispositivo não encontrado.";
                    _statusLabel.ForeColor = Color.OrangeRed;

                    MessageBox.Show(
                        "O dispositivo não foi encontrado.\nVerifica a ligação USB.",
                        "USB HID",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Warning
                    );
                }
            }
            catch (Exception ex)
            {
                _statusLabel.Text = "Estado: Erro na verificação.";
                _statusLabel.ForeColor = Color.OrangeRed;

                MessageBox.Show(
                    ex.Message,
                    "Erro na comunicação HID",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error
                );

using System;
using System.Drawing;
using System.Windows.Forms;
using Abre_o_Olho.App.Services;

namespace Abre_o_Olho.App.Forms
{
    public class MainForm : Form
    {
        private readonly HidService _hidService;

        private Label _titleLabel;
        private Label _statusLabel;
        private Label _deviceLabel;
        private Button _scanButton;

        public MainForm()
        {
            _hidService = new HidService();

            InitializeInterface();
        }

        private void InitializeInterface()
        {
            Text = "Abre o Olho | USB Monitor";
            Size = new Size(650, 400);
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
                Location = new Point(35, 35)
            };

            _statusLabel = new Label
            {
                Text = "Estado: Aguardando verificação",
                Font = new Font("Segoe UI", 12),
                ForeColor = Color.LightGray,
                AutoSize = true,
                Location = new Point(38, 125)
            };

            _deviceLabel = new Label
            {
                Text = "VID: 0x6021 | PID: 0x0001",
                Font = new Font("Segoe UI", 10),
                ForeColor = Color.Silver,
                AutoSize = true,
                Location = new Point(38, 165)
            };

            _scanButton = new Button
            {
                Text = "PROCURAR DISPOSITIVO",
                Size = new Size(240, 45),
                Location = new Point(38, 230),
                BackColor = Color.FromArgb(40, 110, 180),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat,
                Cursor = Cursors.Hand
            };

            _scanButton.FlatAppearance.BorderSize = 0;

            _scanButton.Click += ScanButton_Click;

            Controls.Add(_titleLabel);
            Controls.Add(_statusLabel);
            Controls.Add(_deviceLabel);
            Controls.Add(_scanButton);
        }

        private void ScanButton_Click(object? sender, EventArgs e)
        {
            try
            {
                _statusLabel.Text = "Estado: A estabelecer ligação...";
                _statusLabel.ForeColor = Color.Yellow;

                _scanButton.Enabled = false;

                bool connected = _hidService.OpenConnection();

                if (connected)
                {
                    _statusLabel.Text =
                        "Estado: Comunicação HID estabelecida!";

                    _statusLabel.ForeColor = Color.LightGreen;

                    MessageBox.Show(
                        "Ligação HID estabelecida com sucesso!",
                        "Abre o Olho",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Information
                    );
                }
                else
                {
                    _statusLabel.Text =
                        "Estado: Não foi possível abrir a ligação.";

                    _statusLabel.ForeColor = Color.OrangeRed;

                    MessageBox.Show(
                        "O dispositivo foi localizado, mas não foi possível abrir a comunicação.",
                        "Erro HID",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Warning
                    );
                }
            }
            catch (Exception ex)
            {
                _statusLabel.Text = "Estado: Erro na comunicação.";
                _statusLabel.ForeColor = Color.OrangeRed;

                MessageBox.Show(
                    ex.Message,
                    "Erro HID",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error
                );
            }
            finally
            {
                _scanButton.Enabled = true;
            }
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            _hidService.CloseConnection();

            base.OnFormClosing(e);
        }
    }
}
            }
        }
    }
}

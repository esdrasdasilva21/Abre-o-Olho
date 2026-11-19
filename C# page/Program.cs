
using Abre_o_Olho.App.Services;
using System;
using System.Drawing;
using System.Windows.Forms;

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
                Size = new Size(220, 45),
                Location = new Point(38, 230),
                BackColor = Color.FromArgb(40, 110, 180),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat
            };

            _scanButton.Click += ScanButton_Click;

            Controls.Add(_titleLabel);
            Controls.Add(_statusLabel);
            Controls.Add(_deviceLabel);
            Controls.Add(_scanButton);
        }

        private void ScanButton_Click(object? sender, EventArgs e)
        {
            bool connected = _hidService.IsDeviceConnected();

            if (connected)
            {
                _statusLabel.Text = "Estado: Dispositivo encontrado!";
                _statusLabel.ForeColor = Color.LightGreen;
            }
            else
            {
                _statusLabel.Text = "Estado: Dispositivo não encontrado.";
                _statusLabel.ForeColor = Color.OrangeRed;
            }
        }
    }
}


using System;
using System.Windows.Forms;
using Abre_o_Olho.App.Forms;

namespace Abre_o_Olho.App
{
    internal static class Program
    {
        [STAThread]
        static void Main()
        {
            ApplicationConfiguration.Initialize();
            Application.Run(new MainForm());
        }
    }
}

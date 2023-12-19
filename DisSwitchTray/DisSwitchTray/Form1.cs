using Gma.System.MouseKeyHook;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace DisSwitchTray
{
    public partial class Form1 : Form
    {
        private NotifyIcon trayIcon;
        public Form1()
        {
            InitializeComponent();
            trayIcon = new NotifyIcon
            {
                Icon = Properties.Resources.icon, // Add your icon to the resources
                Text = "TrayApp Example",
                Visible = true
            };
            // Create a context menu for the tray ico
            ContextMenuStrip contextMenu = new ContextMenuStrip();

            ToolStripMenuItem help = new ToolStripMenuItem("Help");
            help.Click += OnHelp;
            contextMenu.Items.Add(help);

           
            ToolStripMenuItem exit = new ToolStripMenuItem("Exit");
            exit.Click += OnExit;
            contextMenu.Items.Add(exit);

            trayIcon.ContextMenuStrip = contextMenu;

            // Set up global key hook for Win + Shift + E
            GlobalKeyboardHook keyboardHook = new GlobalKeyboardHook(this.Handle);
            keyboardHook.RegisterHotKey((int)(Keys.E), (uint)(KeyModifiers.Shift));
            keyboardHook.KeyPress += OnHotKeyPressed;

            // Handle the FormClosing event to clean up when the application is closed
            FormClosing += TrayApp_FormClosing;
        }

        private void OnExit(object sender, EventArgs e)
        {
            // Clean up and exit the application
            trayIcon.Visible = false;
            Application.Exit();
        }

        private void OnHelp(object sender, EventArgs e)
        {
            // Show help information or perform any other action
            MessageBox.Show("Help information goes here.", "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }


        private void OnHotKeyPressed(object sender, KeyPressedEventArgs e)
        {
            // Handle the Win + Shift + E hotkey
            
            MessageBox.Show("Win + Shift + E pressed!", "Hotkey Pressed", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void TrayApp_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Clean up when the application is closed
            trayIcon.Visible = false;
        }

    }
}





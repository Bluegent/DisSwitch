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
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace DisSwitchTray
{
    public class KeyTracker
    {
        public HashSet<Keys> keys = new HashSet<Keys>();
        public Keys[] allowed = { Keys.E, Keys.C, Keys.D, Keys.LWin, Keys.RWin, Keys.LShiftKey, Keys.RShiftKey };
        bool releaseAfterPress = true;

        public void toggle(Keys key, bool add)
        {
            if (!allowed.Contains(key))
            {
                return;
            }
            if (add)
            {
                keys.Add(key);
            }
            else
            {
                keys.Remove(key);
                if (keys.Count < 3)
                {
                    releaseAfterPress = true;
                }
            }
        }
        public bool winPress()
        {
            return keys.Contains(Keys.LWin) || keys.Contains(Keys.RWin);
        }
        public bool shiftPress()
        {
            return keys.Contains(Keys.LShiftKey) || keys.Contains(Keys.RShiftKey);
        }
        public bool validState(Keys key)
        {
            if (!releaseAfterPress)
            {
                return false;
            }
            if (!winPress() || !shiftPress())
            {
                return false;
            }
            if (keys.Count != 3 || !keys.Contains(key))
            {
                return false;
            }
            releaseAfterPress = false;
            return true;
        }
    };
    public partial class Form1 : Form
    {
        private NotifyIcon trayIcon;
        private IKeyboardMouseEvents globalHook;
        private KeyTracker tracker;
        private DateTime lastAction;
        private bool consoleShown = false;
        ToolStripMenuItem debug;

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


            ToolStripMenuItem clone = new ToolStripMenuItem("Clone");
            clone.Click += Clone;
            contextMenu.Items.Add(clone);

            debug = new ToolStripMenuItem("Debug");
            debug.Click += Debug_Click; 
            contextMenu.Items.Add(debug);

            ToolStripMenuItem disc = new ToolStripMenuItem("Disconnect");
            disc.Click += Disconnect;
            contextMenu.Items.Add(disc);

            ToolStripMenuItem extend = new ToolStripMenuItem("Extend");
            extend.Click += Extend;
            contextMenu.Items.Add(extend);

            ToolStripMenuItem exit = new ToolStripMenuItem("Exit");
            exit.Click += OnExit;
            contextMenu.Items.Add(exit);

            trayIcon.ContextMenuStrip = contextMenu;

            tracker = new KeyTracker();
            globalHook = Hook.GlobalEvents();

            globalHook.KeyDown += OnKeyPressed;
            globalHook.KeyUp += OnKeyReleased;
            AllocConsole();
            ShowWindow(GetConsoleWindow(), SW_HIDE);
            FormClosing += TrayApp_FormClosing;
            Shown += Form1_Shown;
            
        }

        private void Debug_Click(object sender, EventArgs e)
        {
            consoleShown = !consoleShown;
            debug.Checked = consoleShown;
            if (consoleShown)
            {
                ShowWindow(GetConsoleWindow(), SW_SHOW);
            }
            else
            {
                ShowWindow(GetConsoleWindow(), SW_HIDE);
            }
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            Hide();
        }

        private void RunProcess(string args)
        {
            ProcessStartInfo processStartInfo = new ProcessStartInfo
            {
                FileName = "DisSwitch.exe",
                Arguments = args,
                UseShellExecute = false, // This must be set to false to redirect input/output
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                CreateNoWindow = true // This will prevent the command prompt window from appearing
            };

            using (Process process = new Process())
            {
                process.StartInfo = processStartInfo;

                // Optionally, you can handle events such as OutputDataReceived and ErrorDataReceived
                process.OutputDataReceived += (sender, e) => Console.WriteLine($"Output: {e.Data}");
                process.ErrorDataReceived += (sender, e) => Console.WriteLine($"Error: {e.Data}");

                process.Start();

                // Start asynchronously reading the output and error streams to avoid deadlocks
                process.BeginOutputReadLine();
                process.BeginErrorReadLine();

                // Wait for the process to exit
                process.WaitForExit();

                // Get the exit code
                int exitCode = process.ExitCode;

                Console.WriteLine($"Exit Code: {exitCode}");
            }
        }
        private void Clone(object sender, EventArgs e)
        {
            Console.WriteLine("clone...");
            RunProcess("clone");
           
        }
        private void Disconnect(object sender, EventArgs e)
        {
            Console.WriteLine("disconnect...");
            RunProcess("disc");
        }

        private void Extend(object sender, EventArgs e)
        {
            Console.WriteLine("extend...");
            RunProcess("extend");
        }

        private bool validTiming()
        {
            DateTime now = DateTime.Now;
            return ((now - lastAction).TotalSeconds > 5);
        }
        private void EvalTracker()
        {
            if (!validTiming())
            {
                return;
            }

            if (tracker.validState(Keys.E))
            {
                lastAction = DateTime.Now;
                Console.WriteLine("win + shift + e pressed");
                Extend(null, null);
                return;
            }
            if (tracker.validState(Keys.D))
            {
                lastAction = DateTime.Now;
                Console.WriteLine("win + shift + d pressed");
                Disconnect(null, null);
                return;
            }
            if (tracker.validState(Keys.C))
            {
                lastAction = DateTime.Now;
                Console.WriteLine("win + shift + c pressed");
                Clone(null, null);
                return;
            }
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
            string helpText = "Shortcuts:" + Environment.NewLine;
            helpText += "win + shift + e - extend to TV" + Environment.NewLine;
            helpText += "win + shift + c - clone main display to TV" + Environment.NewLine;
            helpText += "win + shift + d - disconnect TV" + Environment.NewLine;
            MessageBox.Show(helpText, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }


        private void OnKeyPressed(object sender, KeyEventArgs e)
        {
            tracker.toggle(e.KeyCode, true);
            if (tracker.allowed.Contains(e.KeyCode))
            {
                EvalTracker();
            }
        }

        private void OnKeyReleased(object sender, KeyEventArgs e)
        {
            tracker.toggle(e.KeyCode, false);
            if (tracker.allowed.Contains(e.KeyCode))
            {
                EvalTracker();
            }
        }

        private void TrayApp_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Clean up when the application is closed
            trayIcon.Visible = false;
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool AllocConsole();


        [DllImport("kernel32.dll")]
        static extern IntPtr GetConsoleWindow();

        [DllImport("user32.dll")]
        static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        const int SW_HIDE = 0;
        const int SW_SHOW = 5;

    }
}





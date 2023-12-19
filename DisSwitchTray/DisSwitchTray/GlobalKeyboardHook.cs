using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace DisSwitchTray
{
    public class KeyPressedEventArgs : EventArgs
    {
        public Keys Key { get; }
        public KeyModifiers Modifiers { get; }

        public KeyPressedEventArgs(Keys key, KeyModifiers modifiers)
        {
            Key = key;
            Modifiers = modifiers;
        }
    }
    // Define a class to handle global keyboard events
    public class GlobalKeyboardHook : IDisposable
    {
        public event EventHandler<KeyPressedEventArgs> KeyPress;

        private int keyId;
        private const int WM_HOTKEY = 0x0312;
        private readonly IntPtr formHandle;

        public GlobalKeyboardHook(IntPtr formHandle)
        {
            this.formHandle = formHandle;
            
        }

        public void RegisterHotKey(int key, uint modifier)
        {
            keyId = key;
            if (!NativeMethods.RegisterHotKey(formHandle, keyId, modifier, Keys.None))
            {
                throw new InvalidOperationException("Couldn't register the hotkey.");
            }
            Application.AddMessageFilter(new MessageFilter(this));
        }

        public void UnregisterHotKey()
        {
            NativeMethods.UnregisterHotKey(IntPtr.Zero, keyId);
        }

        public void Dispose()
        {
            UnregisterHotKey();
        }

        private class MessageFilter : IMessageFilter
        {
            private readonly GlobalKeyboardHook hook;

            public MessageFilter(GlobalKeyboardHook hook)
            {
                this.hook = hook;
            }

            public bool PreFilterMessage(ref Message m)
            {
                if (m.Msg == WM_HOTKEY)
                {
                    Keys key = (Keys)(((int)m.LParam >> 16) & 0xFFFF);
                    KeyModifiers modifier = (KeyModifiers)((int)m.LParam & 0xFFFF);

                    if (modifier == KeyModifiers.None && key == Keys.None)
                    {
                        hook.KeyPress?.Invoke(null, new KeyPressedEventArgs(Keys.None, KeyModifiers.None));
                    }
                    else
                    {
                        hook.KeyPress?.Invoke(null, new KeyPressedEventArgs(key, modifier));
                    }

                    return true;
                }

                return false;
            }
        }
    }

    // Define an enumeration for key modifiers
    [Flags]
    public enum KeyModifiers
    {
        None = 0,
        Alt = 1,
        Ctrl = 2,
        Shift = 4,
        Win = 8
    }

    // Define a static class for native methods
    public static class NativeMethods
    {
        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool RegisterHotKey(IntPtr hWnd, int id, uint fsModifiers, Keys vk);

        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool UnregisterHotKey(IntPtr hWnd, int id);
    }

}

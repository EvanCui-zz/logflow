namespace LogFlow.Viewer
{
    using System;
    using System.Collections.Generic;
    using System.Configuration;
    using System.Windows.Forms;
    using LogFlow.Viewer.Properties;

    [SettingsSerializeAs(SettingsSerializeAs.Xml)]
    public class HotKeys : Dictionary<Keys, string>
    {
        public const string ActionFocusPatternBox = "Focus Pattern Box";
        public const string ActionGoto = "Goto";
        public const string ActionFilter = "Filter";
        public const string ActionSearch = "Search";
        public const string ActionBackSearch = "Back Search";
        public const string ActionCount = "Count";
        public const string ActionTag = "Tag";
        public const string ActionCopy = "Copy";
        public const string ActionOpen = "Open";
        public const string ActionFilteredOpen = "Filtered Open";

        public static Dictionary<string, Action> KeyActions;

        private static readonly Lazy<HotKeys> InternalInstance =
            new Lazy<HotKeys>(() => Settings.Default.HotKeys ?? new HotKeys()
            {
                { System.Windows.Forms.Keys.G | System.Windows.Forms.Keys.Control, ActionGoto },
                { System.Windows.Forms.Keys.D | System.Windows.Forms.Keys.Alt, ActionFocusPatternBox },
                { System.Windows.Forms.Keys.D | System.Windows.Forms.Keys.Control, ActionFocusPatternBox },
                { System.Windows.Forms.Keys.F | System.Windows.Forms.Keys.Control, ActionFilter },
                { System.Windows.Forms.Keys.S | System.Windows.Forms.Keys.Control, ActionSearch },
                { System.Windows.Forms.Keys.N | System.Windows.Forms.Keys.Control, ActionCount },
                { System.Windows.Forms.Keys.T | System.Windows.Forms.Keys.Control, ActionTag },
                { System.Windows.Forms.Keys.F1, ActionFilter },
                { System.Windows.Forms.Keys.F3, ActionSearch },
                { System.Windows.Forms.Keys.F3 | System.Windows.Forms.Keys.Shift, ActionBackSearch },
                { System.Windows.Forms.Keys.F4, ActionCount },
                { System.Windows.Forms.Keys.F2, ActionGoto },
                { System.Windows.Forms.Keys.O | System.Windows.Forms.Keys.Control, ActionOpen },
                { System.Windows.Forms.Keys.L | System.Windows.Forms.Keys.Control, ActionFilteredOpen },
                { System.Windows.Forms.Keys.C | System.Windows.Forms.Keys.Control, ActionCopy },
            });

        public static HotKeys Instance => InternalInstance.Value;

        public bool PerformAction(Keys key)
        {
            string name;
            if (!this.TryGetValue(key, out name))
            {
                return false;
            }

            Action action;
            var result = KeyActions.TryGetValue(name, out action);
            action?.Invoke();
            return result;
        }
    }
}
using System;

namespace LogFlow.Viewer
{
    using System.Configuration;
    using System.Drawing;

    [SettingsSerializeAs(SettingsSerializeAs.Xml)]
    public class LevelColors : ICloneable
    {
        public object Clone() => new LevelColors()
        {
            Critical = this.Critical,
            Error = this.Error,
            Warning = this.Warning,
            Info = this.Info,
            Verbose = this.Verbose,
        };

        public Color Critical { get; set; } = Color.Red;
        public Color Error { get; set; } = Color.FromArgb(255, 128, 0);
        public Color Warning { get; set; } = Color.FromArgb(255, 192, 0);
        public Color Info { get; set; }
        public Color Verbose { get; set; }
        public Color Detail { get; set; }
    }
}

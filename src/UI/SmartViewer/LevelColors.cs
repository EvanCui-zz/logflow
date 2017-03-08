using System;
using System.ComponentModel;
using System.Xml.Serialization;

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

        [Browsable(false)]
        public int SerializeCritical
        {
            get { return this.Critical.ToArgb(); }
            set { this.Critical = Color.FromArgb(value); }
        }

        [Browsable(false)]
        public int SerializeError
        {
            get { return this.Error.ToArgb(); }
            set { this.Error = Color.FromArgb(value); }
        }

        [Browsable(false)]
        public int SerializeWarning
        {
            get { return this.Warning.ToArgb(); }
            set { this.Warning = Color.FromArgb(value); }
        }

        [Browsable(false)]
        public int SerializeInfo
        {
            get { return this.Info.ToArgb(); }
            set { this.Info = Color.FromArgb(value); }
        }

        [Browsable(false)]
        public int SerializeVerbose
        {
            get { return this.Verbose.ToArgb(); }
            set { this.Verbose = Color.FromArgb(value); }
        }

        [Browsable(false)]
        public int SerializeDetail
        {
            get { return this.Detail.ToArgb(); }
            set { this.Detail = Color.FromArgb(value); }
        }

        [XmlIgnore]
        public Color Critical { get; set; } = Color.Red;
        [XmlIgnore]
        public Color Error { get; set; } = Color.FromArgb(255, 64, 32);
        [XmlIgnore]
        public Color Warning { get; set; } = Color.FromArgb(255, 192, 0);
        [XmlIgnore]
        public Color Info { get; set; }
        [XmlIgnore]
        public Color Verbose { get; set; }
        [XmlIgnore]
        public Color Detail { get; set; }
    }
}

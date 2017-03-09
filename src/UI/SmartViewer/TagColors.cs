using System;
using System.ComponentModel;
using System.Xml.Serialization;

namespace LogFlow.Viewer
{
    using System.Drawing;

    public class TagColors : ICloneable
    {
        public object Clone() => new TagColors() { Tag1 = this.Tag1, Tag2 = this.Tag2, Tag3 = this.Tag3, };

        [Browsable(false)]
        public int SerializeTag1 { get { return this.Tag1.ToArgb(); } set { this.Tag1 = Color.FromArgb(value); } }
        [Browsable(false)]
        public int SerializeTag2 { get { return this.Tag2.ToArgb(); } set { this.Tag2 = Color.FromArgb(value); } }
        [Browsable(false)]
        public int SerializeTag3 { get { return this.Tag3.ToArgb(); } set { this.Tag3 = Color.FromArgb(value); } }

        [XmlIgnore]
        public Color Tag1 { get; set; } = Color.Cyan;
        [XmlIgnore]
        public Color Tag2 { get; set; } = Color.Fuchsia;
        [XmlIgnore]
        public Color Tag3 { get; set; } = Color.Lime;
    }
}

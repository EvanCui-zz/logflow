using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    internal class FakeLogSource : LogSourceBase<DataItemBase>
    {
        private bool loaded = false;
        private bool isProgressive;
        public FakeLogSource(bool isProgressive)
        {
            this.isProgressive = isProgressive;
        }

        public override IEnumerable<int> Load(IFilter filter = null)
        {
            Random r = new Random();
            if (this.loaded)
            {
                if (this.isProgressive)
                {
                    this.templates.Add(string.Format("string templates {{0}}, {{1}}, testing long string, progressive"));

                    for (int i = 0; i < 2; i++)
                    {
                        var rand = r.Next(100);
                        this.AddItem(new DataItemBase()
                        {
                            Id = i,
                            ThreadId = i % 100,
                            Time = DateTime.UtcNow.AddSeconds(i),
                            TemplateId = r.Next(this.templates.Count),
                            Parameters = new object[] { DateTime.UtcNow, i + 255 },
                            ProcessId = i / 100000,
                            Level = (LogLevel)(1 << (rand < 1 ? 0 : (rand < 5 ? 1 : (rand < 10 ? 2 : (rand < 55 ? 3 : 4))))),
                        });

                        yield return i * 100 / 2;
                    }
                }
                else
                {
                    yield return 100; yield break;
                }
            }

            this.loaded = true;

            for (int i = 0; i < 1000; i++)
            {
                this.templates.Add(string.Format("string templates {{0}}, {{1}}, testing long string, {0}", i));
            }

            int totalCount = 1000000;

            for (int i = 0; i < totalCount; i++)
            {
                var rand = r.Next(100);
                this.AddItem(new DataItemBase()
                {
                    Id = i,
                    ThreadId = i % 100,
                    Time = DateTime.UtcNow.AddSeconds(i),
                    TemplateId = r.Next(this.templates.Count),
                    Parameters = new object[] { DateTime.UtcNow, i + 255 },
                    ProcessId = i / 100000,
                    Level = (LogLevel)(1 << (rand < 1 ? 0 : (rand < 5 ? 1 : (rand < 10 ? 2 : (rand < 55 ? 3 : 4))))),
                });

                if (i % (totalCount / 20) == 0)
                {
                    yield return i * 100 / totalCount;
                }
            }
        }
    }
}

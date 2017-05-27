namespace LogFlow.DataModel
{
    using System;
    using System.Threading;
    using System.Threading.Tasks;

    public class NonReentrantTimer : IDisposable
    {
        private CancellationTokenSource cts;
        private Task mainTask;

        public NonReentrantTimer(Func<CancellationToken, Task> task, int dueTime, int period)
        {
            this.cts = new CancellationTokenSource();
            var token = this.cts.Token;

            this.mainTask = Task.Run(async () =>
            {
                while (!token.IsCancellationRequested)
                {
                    await Task.Delay(dueTime, token);
                    dueTime = period;

                    await task(token);
                }
            }, token);
        }

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool isDispose)
        {
            if (isDispose)
            {
                this.cts.Cancel();
                this.cts.Dispose();
                this.cts = null;

                this.mainTask.GetAwaiter().GetResult();
            }
        }
    }
}

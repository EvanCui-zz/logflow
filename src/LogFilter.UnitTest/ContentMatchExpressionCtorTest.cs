namespace LogFlow.Viewer.LogFilter.UnitTest
{
    using LogFlow.Viewer.LogFilter.Expressions;
    using LogFlow.Viewer.LogFilter.Tokens;

    using Microsoft.VisualStudio.TestTools.UnitTesting;

    [TestClass]
    public class ContentMatchExpressionCtorTest
    {
        [TestMethod]
        public void TestTextMatch1()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken("text"));
            Assert.IsInstanceOfType(res, typeof(TextMatchExpression));
            Assert.AreEqual("\"text\"", res.EvalToString(false));
        }

        [TestMethod]
        public void TestTextMatch2()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken("\"basic text\""));
            Assert.IsInstanceOfType(res, typeof(TextMatchExpression));
            Assert.AreEqual("\"basic text\"", res.EvalToString(false));
        }

        [TestMethod]
        public void TestTextMatch3()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken("c:\"basic text\""));
            Assert.IsInstanceOfType(res, typeof(TextMatchExpression));
            Assert.AreEqual("\"basic text\"", res.EvalToString(false));
        }

        [TestMethod]
        public void TestDatetimeBegin()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken("b:\"1990.09.27 3:00 PM\""));
            Assert.IsInstanceOfType(res, typeof(DatetimeBeginMatchExpression));
            Assert.AreEqual("b:1990-09-27T15:00:00", res.EvalToString(false));
        }

        [TestMethod]
        public void TestDatetimeEnd()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken("e:\"1990.09.27 3:00 PM\""));
            Assert.IsInstanceOfType(res, typeof(DatetimeEndMatchExpression));
            Assert.AreEqual("e:1990-09-27T15:00:00", res.EvalToString(false));
        }

        [TestMethod]
        public void TestPid()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken("p:99"));
            Assert.IsInstanceOfType(res, typeof(ProcessIdMatchExpression));
            Assert.AreEqual("p:99", res.EvalToString(false));
        }

        [TestMethod]
        public void TestTid()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken("t:99"));
            Assert.IsInstanceOfType(res, typeof(ThreadIdMatchExpression));
            Assert.AreEqual("t:99", res.EvalToString(false));
        }

        [TestMethod]
        public void TestLogLevel()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken("l:we"));
            Assert.IsInstanceOfType(res, typeof(LogLevelMatchExpression));
            Assert.AreEqual("l:ew", res.EvalToString(false));
        }

        [TestMethod]
        public void TestRegexMatch1()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken(@"r:\w+"));
            Assert.IsInstanceOfType(res, typeof(RegexMatchExpression));
            Assert.AreEqual(@"r:""\w+""", res.EvalToString(false));
        }

        [TestMethod]
        public void TestRegexMatch2()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken(@"r:""\w+ """""""""""));
            Assert.IsInstanceOfType(res, typeof(RegexMatchExpression));
            Assert.AreEqual(@"r:""\w+ """"""""""", res.EvalToString(false));
        }

        [TestMethod]
        public void TestBeginIdx()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken(@"bi:15"));
            Assert.IsInstanceOfType(res, typeof(BeginIdxExpression));
            Assert.AreEqual(@"bi:15", res.EvalToString(false));
        }

        [TestMethod]
        public void TestEndIdx()
        {
            var res = ContentMatchExpression.CreateContentMatchExpression(new ContentToken(@"ei:18"));
            Assert.IsInstanceOfType(res, typeof(EndIdxExpression));
            Assert.AreEqual(@"ei:18", res.EvalToString(false));
        }
    }
}

namespace LogFilter.UnitTest
{
    using Microsoft.VisualStudio.TestTools.UnitTesting;

    [TestClass]
    public class LogFilterInterpreterUnitTest
    {
        [TestMethod]
        public void TestRightAssociate()
        {
            var a = LogFilterInterpreter.Parse("a && (b || d)");
            string res = a.EvalToString(true);
            Assert.AreEqual("((\"a\") && ((\"b\") || (\"d\")))", res);
        }

        [TestMethod]
        public void TestLeftAssociate()
        {
            var a = LogFilterInterpreter.Parse("a && b || d");
            string res = a.EvalToString(true);
            Assert.AreEqual("(((\"a\") && (\"b\")) || (\"d\"))", res);
        }

        [TestMethod]
        public void TestUnary1()
        {
            var a = LogFilterInterpreter.Parse("a && !b");
            string res = a.EvalToString(true);
            Assert.AreEqual("((\"a\") && (!(\"b\"))) ", res);
        }
    }
}

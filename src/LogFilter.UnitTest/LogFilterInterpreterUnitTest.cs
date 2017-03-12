namespace LogFilter.UnitTest
{
    using Microsoft.VisualStudio.TestTools.UnitTesting;

    [TestClass]
    public class LogFilterInterpreterUnitTest
    {
        [TestMethod]
        public void TestParenthesis1()
        {
            string res = LogFilterInterpreter.Parse("a && (b || d)").EvalToString(true);
            Assert.AreEqual("((\"a\") && (((\"b\") || (\"d\"))))", res);
        }

        [TestMethod]
        public void TestParenthesis2()
        {
            string res = LogFilterInterpreter.Parse("a && (b || d)").EvalToString(false);
            Assert.AreEqual("\"a\" && (\"b\" || \"d\")", res);
        }

        [TestMethod]
        public void TestLeftAssociate()
        {
            string res = LogFilterInterpreter.Parse("a && b || d").EvalToString(true);
            Assert.AreEqual("(((\"a\") && (\"b\")) || (\"d\"))", res);
        }

        [TestMethod]
        public void TestUnary1()
        {
            string res = LogFilterInterpreter.Parse("a && !b").EvalToString(true);
            Assert.AreEqual("((\"a\") && (!(\"b\")))", res);
        }

        [TestMethod]
        public void TestUnary2()
        {
            string res = LogFilterInterpreter.Parse("a && !!b").EvalToString(true);
            Assert.AreEqual("((\"a\") && (!(!(\"b\"))))", res);
        }

        [TestMethod]
        public void TestUnary3()
        {
            string res = LogFilterInterpreter.Parse("a && !(!b)").EvalToString(true);
            Assert.AreEqual("((\"a\") && (!((!(\"b\")))))", res);
        }

        [TestMethod]
        public void TestGetParseResult1()
        {
            var res = LogFilterInterpreter.GetFilterParseResult("a");
            Assert.AreEqual(true, res.Succeed);
            Assert.AreEqual("a", res.Input);
            Assert.AreEqual("(\"a\")", res.Ast.EvalToString(true));
        }

        [TestMethod]
        public void TestGetParseResult2()
        {
            var res = LogFilterInterpreter.GetFilterParseResult("a || ");
            Assert.AreEqual(false, res.Succeed);
            Assert.AreEqual("a || ", res.Input);
        }
    }
}

using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using OpenQA.Selenium;
using OpenQA.Selenium.Chrome;
using OpenQA.Selenium.Support.UI;

namespace Selenium_Prac1
{
    [TestClass]
    public class UnitTest1
    {
        ChromeDriver ChromeDriver;

        [TestMethod]
        public void TestMethod1()
        {
            ChromeDriver = new ChromeDriver();
            ChromeDriver.Navigate().GoToUrl("http://www.google.com/");
            ChromeDriver.FindElementById("gbqfq").SendKeys("Google");
            ChromeDriver.FindElementById("gbqfq").SendKeys(Keys.Enter);
            System.Threading.Thread.Sleep(500);
        }

        [TestCleanup]

        public void Shutdown()
        {
            ChromeDriver.Quit();
        }
    }
}

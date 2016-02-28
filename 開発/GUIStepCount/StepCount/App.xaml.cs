using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using StepCount.Views;

namespace StepCount
{
    /// <summary>
    /// App.xaml の相互作用ロジック
    /// </summary>
    public partial class App : Application
    {
        private ResourceDictionary dictionary;
        private void Application_Startup(object sender, StartupEventArgs e) {
            var v = new MainView();
            v.Show();
        }

        public void ChangeTheme(string themeName) {
            if(dictionary == null) {
                dictionary = new ResourceDictionary();
                Application.Current.Resources.MergedDictionaries.Add(dictionary);
            }

            string themeUrl = String.Format("Themes/{0}.xaml", themeName);
            dictionary.Source = new Uri(themeUrl);
        }
    }
}

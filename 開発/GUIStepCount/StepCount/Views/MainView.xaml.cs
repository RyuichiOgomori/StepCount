using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using StepCount.Models;

namespace StepCount.Views
{
    /// <summary>
    /// MainView.xaml の相互作用ロジック
    /// </summary>
    public partial class MainView : Window
    {
        public MainView() {
            InitializeComponent();
        }

        private void FileDrop(object sender, DragEventArgs e) {
            var textBox = sender as TextBox;
            string PathString = string.Empty;
            string[] files = e.Data.GetData(DataFormats.FileDrop) as string[];
            if(files != null) {
                foreach(var s in files) {
                    if(string.IsNullOrEmpty(PathString)) {
                        PathString += s;
                    } else {
                        PathString += "," + s;
                    }
                }
                textBox.Text = PathString;
            }
        }

        private void FilePreviewDragOver(object sender, DragEventArgs e) {
            if(e.Data.GetDataPresent(DataFormats.FileDrop, true))
                e.Effects = DragDropEffects.Copy;
            else
                e.Effects = DragDropEffects.None;
            e.Handled = true;
        }
    }
}

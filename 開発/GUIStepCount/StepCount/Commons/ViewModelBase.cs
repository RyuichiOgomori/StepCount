using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace StepCount.Commons
{
    /// <summary>
    /// ViewModelの基本クラス
    /// </summary>
    class ViewModelBase : INotifyPropertyChanged
    {
        /// <summary> 
        /// プロパティの変更
        /// </summary> 
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary> 
        /// PropertyChangedイベント
        /// </summary> 
        /// <param name="propertyName">プロパティ名</param> 
        protected virtual void RaisePropertyChanged(string propertyName) {
            var h = this.PropertyChanged;
            if (h != null) {
                h(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }
}

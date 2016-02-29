using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StepCount.Models
{
    class ResultFile
    {
        public string Path { get; set; }
        public string Name { get; set; }
        public int Step { get; set; }
        public int Comment { get; set; }
        public int Space { get; set; }
        public int All{ get; set; }
    }
}

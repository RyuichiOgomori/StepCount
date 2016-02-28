using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Configuration;

namespace StepCount.Models
{
    class LineCheck
    {

        /* コメント開始チェック */
        public static bool IsStartCommentLine(string line) {
            var str = line.Trim();
            if(string.IsNullOrEmpty(ConfigurationManager.AppSettings["CommentStart"])) return false;
            var CommentStartStrings = ConfigurationManager.AppSettings["CommentStart"].Split(',');
            foreach(var commentStart in CommentStartStrings) {
                if(0 == str.IndexOf(commentStart)) {
                    return true;
                }
            }
            return false;
        }

        /* コメント終了チェック */
        public static bool IsEndCommentLine(string line) {
            var str = line.Trim();
            if(string.IsNullOrEmpty(ConfigurationManager.AppSettings["CommentEnd"])) return false;
            var CommentEndStrings = ConfigurationManager.AppSettings["CommentEnd"].Split(',');
            foreach(var commentEnd in CommentEndStrings) {
                if(str.Length - commentEnd.Length == str.IndexOf(commentEnd)) {
                    return true;
                }
            }
            return false;
        }

        /* コメント行チェック */
        public static bool IsCommentLine(string line) {
            var str = line.Trim();
            if(string.IsNullOrEmpty(ConfigurationManager.AppSettings["Comment"])) return false;
            var CommentStrings = ConfigurationManager.AppSettings["Comment"].Split(',');
            foreach(var comment in CommentStrings) {
                if(0 == str.IndexOf(comment)) {
                    return true;
                }
            }
            return false;
        }

        /* バルク開始チェック */
        public static int IsStartBulkLine(string line) {
            var str = line.Trim();
            if(string.IsNullOrEmpty(ConfigurationManager.AppSettings["BulkStart"])) return -1;
            if(string.IsNullOrEmpty(ConfigurationManager.AppSettings["BulkQuantity"])) return -1;
            var BulkStartStrings = ConfigurationManager.AppSettings["BulkStart"].Split(',');
            var BulkQuantity = Array.ConvertAll(ConfigurationManager.AppSettings["BulkQuantity"].Split(',')
                , new Converter<string, int>(s => int.Parse(s)));

            foreach(var bulkStart in BulkStartStrings) {
                if(0 == str.IndexOf(bulkStart)) {
                    var index = Array.IndexOf(BulkStartStrings, bulkStart);
                    return BulkQuantity[index];
                }
            }
            return -1;
        }

        /* バルク終了チェック */
        public static bool IsEndBulkLine(string line) {
            var str = line.Trim();
            if(string.IsNullOrEmpty(ConfigurationManager.AppSettings["BulkEnd"])) return false;
            var BulkEndStrings = ConfigurationManager.AppSettings["BulkEnd"].Split(',');
            foreach(var bulkEnd in BulkEndStrings) {
                if(str.Length - bulkEnd.Length == str.IndexOf(bulkEnd)) {
                    return true;
                }
            }
            return false;
        }

        /* 空白行チェック */
        public static bool IsSpaceLine(string str) {
            for(int i = 0; i < str.Length; i++) {
                if(' ' != str[i] && '　' != str[i] && '\t' != str[i]) {
                    return false;
                }
            }
            return true;
        }
    }
}

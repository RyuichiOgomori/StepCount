using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using StepCount.Commons;
using StepCount.Models;
using System.Windows.Forms;
using Microsoft.Win32;
using System.IO;

namespace StepCount.ViewModels
{
    class MainViewModel : ViewModelBase
    {
        #region property



        private string selectedThemeName;
        /// <summary>
        /// 選択したThemeの名前を取得または設定する。
        /// </summary>
        public string SelectedThemeName {
            get { return selectedThemeName; }
            set {
                if(selectedThemeName == value) return;
                selectedThemeName = value;
                RaisePropertyChanged("SelectedThemeName");
            }
        }

        private string directoryPath;
        /// <summary>
        /// ディレクトリのパスを取得または設定する。
        /// </summary>
        public string DirectoryPath {
            get { return directoryPath; }
            set {
                if (directoryPath == value) return;
                directoryPath = value;
                RaisePropertyChanged("DirectoryPath");
            }
        }

        private string[] filePaths;
        /// <summary>
        /// ファイルのパスを取得または設定する。
        /// </summary>
        public string[] FilePaths {
            get { return filePaths; }
            set {
                if(filePaths == value) return;
                filePaths = value;
                RaisePropertyChanged("FilePaths");
            }
        }

        private string targetPath;
        /// <summary>
        /// ファイルのパスを取得または設定する。
        /// </summary>
        public string TargetPath {
            get { return targetPath; }
            set {
                if(targetPath == value) return;
                targetPath = value;
                RaisePropertyChanged("TargetPath");
            }
        }

        private int stepQuantity;
        /// <summary>
        /// カウントしたステップ数を取得または設定する。
        /// </summary>
        public int StepQuantity {
            get { return stepQuantity; }
            set {
                if(stepQuantity == value) return;
                stepQuantity = value;
                RaisePropertyChanged("StepQuantity");
            }
        }

        private int spaceQuantity;
        /// <summary>
        /// カウントした空白行数を取得または設定する。
        /// </summary>
        public int SpaceQuantity {
            get { return spaceQuantity; }
            set {
                if(spaceQuantity == value) return;
                spaceQuantity = value;
                RaisePropertyChanged("SpaceQuantity");
            }
        }

        private int commentQuantity;
        /// <summary>
        /// カウントしたコメント行を取得または設定する。
        /// </summary>
        public int CommentQuantity {
            get { return commentQuantity; }
            set {
                if(commentQuantity == value) return;
                commentQuantity = value;
                RaisePropertyChanged("CommentQuantity");
            }
        }

        private int allQuantity;
        /// <summary>
        /// カウントした全体行数を取得または設定する。
        /// </summary>
        public int AllQuantity {
            get { return allQuantity; }
            set {
                if(allQuantity == value) return;
                allQuantity = value;
                RaisePropertyChanged("AllQuantity");
            }
        }

        private string fileName;
        /// <summary>
        /// ファイル名を取得または設定する。
        /// </summary>
        public string FileName {
            get { return fileName; }
            set {
                if (fileName == value) return;
                fileName = value;
                RaisePropertyChanged("FileName");
            }
        }

        private string fileType = "*.*";
        /// <summary>
        /// 拡張子を取得または設定する。
        /// </summary>
        public string FileType {
            get { return fileType; }
            set {
                if(fileType == value) return;
                fileType = value;
                RaisePropertyChanged("FileType");
            }
        }

        private bool targetDirectory = false;
        /// <summary>
        /// 検索対象をディレクトリ内かどうかを取得または設定する。
        /// </summary>
        public bool TargetDiretory {
            get { return targetDirectory; }
            set {
                if(targetDirectory == value) return;
                targetDirectory = value;
                if(!targetDirectory)
                    IsDeepDirectoryCount = targetDirectory;
                RaisePropertyChanged("TargetDiretory");
            }
        }

        private bool isDeepDirectoryCount = false;
        /// <summary>
        /// ディレクトリを再帰的に検索するかどうかを取得または設定する。
        /// </summary>
        public bool IsDeepDirectoryCount {
            get { return isDeepDirectoryCount; }
            set {
                if(isDeepDirectoryCount == value) return;
                isDeepDirectoryCount = value;
                TargetDiretory = isDeepDirectoryCount;
                RaisePropertyChanged("IsDeepDirectoryCount");
            }
        }
        #endregion

        #region Command
        private ICommand referenceCommand;
        /// <summary>
        /// 参照ボタン
        /// </summary>
        public ICommand ReferenceCommand {
            get {
                if (referenceCommand == null)
                    referenceCommand = new RelayCommand(() =>
                    {
                        if(TargetDiretory) {
                            // ディレクトリのパスを取得
                            FolderBrowserDialog dialog = new FolderBrowserDialog();
                            dialog.ShowNewFolderButton = false;
                            dialog.ShowDialog();
                            DirectoryPath = dialog.SelectedPath;
                            TargetPath = DirectoryPath;
                        } else {
                            // ファイルパスを取得
                            var dialog = new Microsoft.Win32.OpenFileDialog();
                            dialog.Multiselect = true;
                            dialog.ShowDialog();
                            FilePaths = dialog.FileNames;
                            BondFilePath(FilePaths);
                        }
                    });
                return referenceCommand;
            }
        }

        private ICommand countCommand;
        /// <summary>
        /// カウントボタン
        /// </summary>
        public ICommand CountCommand {
            get {
                if(countCommand == null)
                    countCommand = new RelayCommand(() =>
                    {
                        string line;
                        AllQuantity = 0;
                        SpaceQuantity = 0;
                        StepQuantity = 0;
                        CommentQuantity = 0;
                        string[] files;

                        if(string.IsNullOrEmpty(DirectoryPath) && FilePaths == null) return;

                        if(TargetDiretory) {
                            if(string.IsNullOrEmpty(DirectoryPath)) return;
                            files = Directory.GetFiles(DirectoryPath,FileType);
                            if(IsDeepDirectoryCount)
                                files = Directory.GetFiles(DirectoryPath, FileType, SearchOption.AllDirectories);
                        } else {
                            if(FilePaths.Length == 0) return;
                            files = FilePaths;
                        }

                        // カウント
                        foreach(string file in files) {
                            StreamReader sr = new StreamReader(file);

                            while(sr.Peek() >= 0) {
                                line = sr.ReadLine();
                                AllQuantity++;
                                if(LineCheck.IsStartCommentLine(line)) {
                                    CommentQuantity++;
                                    while(sr.Peek() >= 0 && !LineCheck.IsEndCommentLine(line)) {
                                        line = sr.ReadLine();
                                        CommentQuantity++;
                                        AllQuantity++;
                                        if(LineCheck.IsSpaceLine(line)) SpaceQuantity++;
                                    }
                                } else if(LineCheck.IsCommentLine(line)) {
                                    CommentQuantity++;
                                } else if(LineCheck.IsSpaceLine(line)) {
                                    SpaceQuantity++;
                                } else if(LineCheck.IsStartBulkLine(line) != -1) {
                                    StepQuantity += LineCheck.IsStartBulkLine(line);
                                    while(sr.Peek() >= 0 && !LineCheck.IsEndBulkLine(line)) {
                                        line = sr.ReadLine();
                                        AllQuantity++;
                                        if(LineCheck.IsSpaceLine(line)) SpaceQuantity++;
                                    }
                                } else {
                                    StepQuantity++;
                                }
                            }
                            sr.Close();
                        }
                    });
                return countCommand;
            }
        }
        #endregion

        #region Method
        public void BondFilePath(string[] paths) {
            for(int i = 0; i < paths.Length; i++) {
                if(i == 0) {
                    TargetPath = paths[i];
                } else {
                    TargetPath += "," + paths[i];
                }
            }
        }
        #endregion
    }
}

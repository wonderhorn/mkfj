namespace MapEditor
{
    partial class MainForm
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.ファイルToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.マップを保存ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.マップを開くToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.新規作成ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ビルボード選択ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.画像として保存ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.編集ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.表示ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.画像データ再読み込みToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ファイルToolStripMenuItem,
            this.編集ToolStripMenuItem,
            this.表示ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(624, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // ファイルToolStripMenuItem
            // 
            this.ファイルToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.マップを保存ToolStripMenuItem,
            this.マップを開くToolStripMenuItem,
            this.新規作成ToolStripMenuItem,
            this.ビルボード選択ToolStripMenuItem,
            this.画像として保存ToolStripMenuItem});
            this.ファイルToolStripMenuItem.Name = "ファイルToolStripMenuItem";
            this.ファイルToolStripMenuItem.Size = new System.Drawing.Size(53, 20);
            this.ファイルToolStripMenuItem.Text = "ファイル";
            // 
            // マップを保存ToolStripMenuItem
            // 
            this.マップを保存ToolStripMenuItem.Name = "マップを保存ToolStripMenuItem";
            this.マップを保存ToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
            this.マップを保存ToolStripMenuItem.Text = "マップを保存";
            this.マップを保存ToolStripMenuItem.Click += new System.EventHandler(this.マップを保存ToolStripMenuItem_Click);
            // 
            // マップを開くToolStripMenuItem
            // 
            this.マップを開くToolStripMenuItem.Name = "マップを開くToolStripMenuItem";
            this.マップを開くToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
            this.マップを開くToolStripMenuItem.Text = "マップを開く";
            this.マップを開くToolStripMenuItem.Click += new System.EventHandler(this.マップを開くToolStripMenuItem_Click);
            // 
            // 新規作成ToolStripMenuItem
            // 
            this.新規作成ToolStripMenuItem.Name = "新規作成ToolStripMenuItem";
            this.新規作成ToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
            this.新規作成ToolStripMenuItem.Text = "新規作成";
            this.新規作成ToolStripMenuItem.Click += new System.EventHandler(this.新規作成ToolStripMenuItem_Click);
            // 
            // ビルボード選択ToolStripMenuItem
            // 
            this.ビルボード選択ToolStripMenuItem.Name = "ビルボード選択ToolStripMenuItem";
            this.ビルボード選択ToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
            this.ビルボード選択ToolStripMenuItem.Text = "ビルボード選択ウィンドウ";
            this.ビルボード選択ToolStripMenuItem.Click += new System.EventHandler(this.ビルボード選択ToolStripMenuItem_Click);
            // 
            // 画像として保存ToolStripMenuItem
            // 
            this.画像として保存ToolStripMenuItem.Name = "画像として保存ToolStripMenuItem";
            this.画像として保存ToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
            this.画像として保存ToolStripMenuItem.Text = "画像にエクスポート";
            this.画像として保存ToolStripMenuItem.Click += new System.EventHandler(this.画像として保存ToolStripMenuItem_Click);
            // 
            // 編集ToolStripMenuItem
            // 
            this.編集ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.画像データ再読み込みToolStripMenuItem});
            this.編集ToolStripMenuItem.Name = "編集ToolStripMenuItem";
            this.編集ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.編集ToolStripMenuItem.Text = "編集";
            // 
            // 表示ToolStripMenuItem
            // 
            this.表示ToolStripMenuItem.Name = "表示ToolStripMenuItem";
            this.表示ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.表示ToolStripMenuItem.Text = "表示";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 419);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(624, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(118, 17);
            this.toolStripStatusLabel1.Text = "toolStripStatusLabel1";
            // 
            // tabControl1
            // 
            this.tabControl1.Location = new System.Drawing.Point(0, 27);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(624, 389);
            this.tabControl1.TabIndex = 2;
            this.tabControl1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.tabControl1_MouseClick);
            // 
            // 画像データ再読み込みToolStripMenuItem
            // 
            this.画像データ再読み込みToolStripMenuItem.Name = "画像データ再読み込みToolStripMenuItem";
            this.画像データ再読み込みToolStripMenuItem.Size = new System.Drawing.Size(182, 22);
            this.画像データ再読み込みToolStripMenuItem.Text = "画像データ再読み込み";
            this.画像データ再読み込みToolStripMenuItem.Click += new System.EventHandler(this.画像データ再読み込みToolStripMenuItem_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(624, 441);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "MapEditor";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem ファイルToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 編集ToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.ToolStripMenuItem 表示ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem マップを保存ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem マップを開くToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 新規作成ToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripMenuItem ビルボード選択ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 画像として保存ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 画像データ再読み込みToolStripMenuItem;
    }
}


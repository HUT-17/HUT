<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Multiple
    Inherits System.Windows.Forms.Form

    'Form 重写 Dispose，以清理组件列表。
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Windows 窗体设计器所必需的
    Private components As System.ComponentModel.IContainer

    '注意: 以下过程是 Windows 窗体设计器所必需的
    '可以使用 Windows 窗体设计器修改它。
    '不要使用代码编辑器修改它。
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Multiple))
        Me.GroupBoxMultiCamera = New System.Windows.Forms.GroupBox
        Me.PictureBoxCamera3 = New System.Windows.Forms.PictureBox
        Me.PictureBoxCamera2 = New System.Windows.Forms.PictureBox
        Me.PictureBoxCamera1 = New System.Windows.Forms.PictureBox
        Me.PictureBoxCamera0 = New System.Windows.Forms.PictureBox
        Me.GroupBoxInit = New System.Windows.Forms.GroupBox
        Me.TextBoxCameraUsingNum = New System.Windows.Forms.TextBox
        Me.Label2 = New System.Windows.Forms.Label
        Me.ButtonInitCamera = New System.Windows.Forms.Button
        Me.TextBoxEnumCameraNum = New System.Windows.Forms.TextBox
        Me.Label1 = New System.Windows.Forms.Label
        Me.GroupBoxSetParam = New System.Windows.Forms.GroupBox
        Me.ButtonSetParam = New System.Windows.Forms.Button
        Me.TextBoxGain = New System.Windows.Forms.TextBox
        Me.LabelGain = New System.Windows.Forms.Label
        Me.TextBoxExposureTime = New System.Windows.Forms.TextBox
        Me.Label3 = New System.Windows.Forms.Label
        Me.GroupBoxImageControl = New System.Windows.Forms.GroupBox
        Me.ButtonClose = New System.Windows.Forms.Button
        Me.ButtonSoftwareOnce = New System.Windows.Forms.Button
        Me.CheckBoxSoftware = New System.Windows.Forms.CheckBox
        Me.ButtonSaveImage = New System.Windows.Forms.Button
        Me.ButtonStopGrabbing = New System.Windows.Forms.Button
        Me.ButtonStartGrabbing = New System.Windows.Forms.Button
        Me.GroupBoxShowControl = New System.Windows.Forms.GroupBox
        Me.Label9 = New System.Windows.Forms.Label
        Me.Label8 = New System.Windows.Forms.Label
        Me.Label7 = New System.Windows.Forms.Label
        Me.Label6 = New System.Windows.Forms.Label
        Me.TextBoxCamera3FrameLose = New System.Windows.Forms.TextBox
        Me.TextBoxCamera2FrameLose = New System.Windows.Forms.TextBox
        Me.TextBoxCamera3FrameGet = New System.Windows.Forms.TextBox
        Me.TextBoxCamera1FrameLose = New System.Windows.Forms.TextBox
        Me.TextBoxCamera2FrameGet = New System.Windows.Forms.TextBox
        Me.TextBoxCamera1FrameGet = New System.Windows.Forms.TextBox
        Me.TextBoxCamera0FrameLose = New System.Windows.Forms.TextBox
        Me.TextBoxCamera0FrameGet = New System.Windows.Forms.TextBox
        Me.Label5 = New System.Windows.Forms.Label
        Me.Label4 = New System.Windows.Forms.Label
        Me.TimerCameraControl = New System.Windows.Forms.Timer(Me.components)
        Me.GroupBoxMultiCamera.SuspendLayout()
        CType(Me.PictureBoxCamera3, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PictureBoxCamera2, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PictureBoxCamera1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PictureBoxCamera0, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.GroupBoxInit.SuspendLayout()
        Me.GroupBoxSetParam.SuspendLayout()
        Me.GroupBoxImageControl.SuspendLayout()
        Me.GroupBoxShowControl.SuspendLayout()
        Me.SuspendLayout()
        '
        'GroupBoxMultiCamera
        '
        Me.GroupBoxMultiCamera.AccessibleDescription = Nothing
        Me.GroupBoxMultiCamera.AccessibleName = Nothing
        resources.ApplyResources(Me.GroupBoxMultiCamera, "GroupBoxMultiCamera")
        Me.GroupBoxMultiCamera.BackgroundImage = Nothing
        Me.GroupBoxMultiCamera.Controls.Add(Me.PictureBoxCamera3)
        Me.GroupBoxMultiCamera.Controls.Add(Me.PictureBoxCamera2)
        Me.GroupBoxMultiCamera.Controls.Add(Me.PictureBoxCamera1)
        Me.GroupBoxMultiCamera.Controls.Add(Me.PictureBoxCamera0)
        Me.GroupBoxMultiCamera.Font = Nothing
        Me.GroupBoxMultiCamera.Name = "GroupBoxMultiCamera"
        Me.GroupBoxMultiCamera.TabStop = False
        '
        'PictureBoxCamera3
        '
        Me.PictureBoxCamera3.AccessibleDescription = Nothing
        Me.PictureBoxCamera3.AccessibleName = Nothing
        resources.ApplyResources(Me.PictureBoxCamera3, "PictureBoxCamera3")
        Me.PictureBoxCamera3.BackgroundImage = Nothing
        Me.PictureBoxCamera3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.PictureBoxCamera3.Font = Nothing
        Me.PictureBoxCamera3.ImageLocation = Nothing
        Me.PictureBoxCamera3.Name = "PictureBoxCamera3"
        Me.PictureBoxCamera3.TabStop = False
        '
        'PictureBoxCamera2
        '
        Me.PictureBoxCamera2.AccessibleDescription = Nothing
        Me.PictureBoxCamera2.AccessibleName = Nothing
        resources.ApplyResources(Me.PictureBoxCamera2, "PictureBoxCamera2")
        Me.PictureBoxCamera2.BackgroundImage = Nothing
        Me.PictureBoxCamera2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.PictureBoxCamera2.Font = Nothing
        Me.PictureBoxCamera2.ImageLocation = Nothing
        Me.PictureBoxCamera2.Name = "PictureBoxCamera2"
        Me.PictureBoxCamera2.TabStop = False
        '
        'PictureBoxCamera1
        '
        Me.PictureBoxCamera1.AccessibleDescription = Nothing
        Me.PictureBoxCamera1.AccessibleName = Nothing
        resources.ApplyResources(Me.PictureBoxCamera1, "PictureBoxCamera1")
        Me.PictureBoxCamera1.BackgroundImage = Nothing
        Me.PictureBoxCamera1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.PictureBoxCamera1.Font = Nothing
        Me.PictureBoxCamera1.ImageLocation = Nothing
        Me.PictureBoxCamera1.Name = "PictureBoxCamera1"
        Me.PictureBoxCamera1.TabStop = False
        '
        'PictureBoxCamera0
        '
        Me.PictureBoxCamera0.AccessibleDescription = Nothing
        Me.PictureBoxCamera0.AccessibleName = Nothing
        resources.ApplyResources(Me.PictureBoxCamera0, "PictureBoxCamera0")
        Me.PictureBoxCamera0.BackgroundImage = Nothing
        Me.PictureBoxCamera0.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.PictureBoxCamera0.Font = Nothing
        Me.PictureBoxCamera0.ImageLocation = Nothing
        Me.PictureBoxCamera0.Name = "PictureBoxCamera0"
        Me.PictureBoxCamera0.TabStop = False
        '
        'GroupBoxInit
        '
        Me.GroupBoxInit.AccessibleDescription = Nothing
        Me.GroupBoxInit.AccessibleName = Nothing
        resources.ApplyResources(Me.GroupBoxInit, "GroupBoxInit")
        Me.GroupBoxInit.BackgroundImage = Nothing
        Me.GroupBoxInit.Controls.Add(Me.TextBoxCameraUsingNum)
        Me.GroupBoxInit.Controls.Add(Me.Label2)
        Me.GroupBoxInit.Controls.Add(Me.ButtonInitCamera)
        Me.GroupBoxInit.Controls.Add(Me.TextBoxEnumCameraNum)
        Me.GroupBoxInit.Controls.Add(Me.Label1)
        Me.GroupBoxInit.Font = Nothing
        Me.GroupBoxInit.Name = "GroupBoxInit"
        Me.GroupBoxInit.TabStop = False
        '
        'TextBoxCameraUsingNum
        '
        Me.TextBoxCameraUsingNum.AccessibleDescription = Nothing
        Me.TextBoxCameraUsingNum.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxCameraUsingNum, "TextBoxCameraUsingNum")
        Me.TextBoxCameraUsingNum.BackgroundImage = Nothing
        Me.TextBoxCameraUsingNum.Font = Nothing
        Me.TextBoxCameraUsingNum.Name = "TextBoxCameraUsingNum"
        '
        'Label2
        '
        Me.Label2.AccessibleDescription = Nothing
        Me.Label2.AccessibleName = Nothing
        resources.ApplyResources(Me.Label2, "Label2")
        Me.Label2.Font = Nothing
        Me.Label2.Name = "Label2"
        '
        'ButtonInitCamera
        '
        Me.ButtonInitCamera.AccessibleDescription = Nothing
        Me.ButtonInitCamera.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonInitCamera, "ButtonInitCamera")
        Me.ButtonInitCamera.BackgroundImage = Nothing
        Me.ButtonInitCamera.Font = Nothing
        Me.ButtonInitCamera.Name = "ButtonInitCamera"
        Me.ButtonInitCamera.UseVisualStyleBackColor = True
        '
        'TextBoxEnumCameraNum
        '
        Me.TextBoxEnumCameraNum.AccessibleDescription = Nothing
        Me.TextBoxEnumCameraNum.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxEnumCameraNum, "TextBoxEnumCameraNum")
        Me.TextBoxEnumCameraNum.BackgroundImage = Nothing
        Me.TextBoxEnumCameraNum.Font = Nothing
        Me.TextBoxEnumCameraNum.Name = "TextBoxEnumCameraNum"
        Me.TextBoxEnumCameraNum.ReadOnly = True
        '
        'Label1
        '
        Me.Label1.AccessibleDescription = Nothing
        Me.Label1.AccessibleName = Nothing
        resources.ApplyResources(Me.Label1, "Label1")
        Me.Label1.Font = Nothing
        Me.Label1.Name = "Label1"
        '
        'GroupBoxSetParam
        '
        Me.GroupBoxSetParam.AccessibleDescription = Nothing
        Me.GroupBoxSetParam.AccessibleName = Nothing
        resources.ApplyResources(Me.GroupBoxSetParam, "GroupBoxSetParam")
        Me.GroupBoxSetParam.BackgroundImage = Nothing
        Me.GroupBoxSetParam.Controls.Add(Me.ButtonSetParam)
        Me.GroupBoxSetParam.Controls.Add(Me.TextBoxGain)
        Me.GroupBoxSetParam.Controls.Add(Me.LabelGain)
        Me.GroupBoxSetParam.Controls.Add(Me.TextBoxExposureTime)
        Me.GroupBoxSetParam.Controls.Add(Me.Label3)
        Me.GroupBoxSetParam.Font = Nothing
        Me.GroupBoxSetParam.Name = "GroupBoxSetParam"
        Me.GroupBoxSetParam.TabStop = False
        '
        'ButtonSetParam
        '
        Me.ButtonSetParam.AccessibleDescription = Nothing
        Me.ButtonSetParam.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonSetParam, "ButtonSetParam")
        Me.ButtonSetParam.BackgroundImage = Nothing
        Me.ButtonSetParam.Font = Nothing
        Me.ButtonSetParam.Name = "ButtonSetParam"
        Me.ButtonSetParam.UseVisualStyleBackColor = True
        '
        'TextBoxGain
        '
        Me.TextBoxGain.AccessibleDescription = Nothing
        Me.TextBoxGain.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxGain, "TextBoxGain")
        Me.TextBoxGain.BackgroundImage = Nothing
        Me.TextBoxGain.Font = Nothing
        Me.TextBoxGain.Name = "TextBoxGain"
        '
        'LabelGain
        '
        Me.LabelGain.AccessibleDescription = Nothing
        Me.LabelGain.AccessibleName = Nothing
        resources.ApplyResources(Me.LabelGain, "LabelGain")
        Me.LabelGain.Font = Nothing
        Me.LabelGain.Name = "LabelGain"
        '
        'TextBoxExposureTime
        '
        Me.TextBoxExposureTime.AccessibleDescription = Nothing
        Me.TextBoxExposureTime.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxExposureTime, "TextBoxExposureTime")
        Me.TextBoxExposureTime.BackgroundImage = Nothing
        Me.TextBoxExposureTime.Font = Nothing
        Me.TextBoxExposureTime.Name = "TextBoxExposureTime"
        '
        'Label3
        '
        Me.Label3.AccessibleDescription = Nothing
        Me.Label3.AccessibleName = Nothing
        resources.ApplyResources(Me.Label3, "Label3")
        Me.Label3.Font = Nothing
        Me.Label3.Name = "Label3"
        '
        'GroupBoxImageControl
        '
        Me.GroupBoxImageControl.AccessibleDescription = Nothing
        Me.GroupBoxImageControl.AccessibleName = Nothing
        resources.ApplyResources(Me.GroupBoxImageControl, "GroupBoxImageControl")
        Me.GroupBoxImageControl.BackgroundImage = Nothing
        Me.GroupBoxImageControl.Controls.Add(Me.ButtonClose)
        Me.GroupBoxImageControl.Controls.Add(Me.ButtonSoftwareOnce)
        Me.GroupBoxImageControl.Controls.Add(Me.CheckBoxSoftware)
        Me.GroupBoxImageControl.Controls.Add(Me.ButtonSaveImage)
        Me.GroupBoxImageControl.Controls.Add(Me.ButtonStopGrabbing)
        Me.GroupBoxImageControl.Controls.Add(Me.ButtonStartGrabbing)
        Me.GroupBoxImageControl.Font = Nothing
        Me.GroupBoxImageControl.Name = "GroupBoxImageControl"
        Me.GroupBoxImageControl.TabStop = False
        '
        'ButtonClose
        '
        Me.ButtonClose.AccessibleDescription = Nothing
        Me.ButtonClose.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonClose, "ButtonClose")
        Me.ButtonClose.BackgroundImage = Nothing
        Me.ButtonClose.Font = Nothing
        Me.ButtonClose.Name = "ButtonClose"
        Me.ButtonClose.UseVisualStyleBackColor = True
        '
        'ButtonSoftwareOnce
        '
        Me.ButtonSoftwareOnce.AccessibleDescription = Nothing
        Me.ButtonSoftwareOnce.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonSoftwareOnce, "ButtonSoftwareOnce")
        Me.ButtonSoftwareOnce.BackgroundImage = Nothing
        Me.ButtonSoftwareOnce.Font = Nothing
        Me.ButtonSoftwareOnce.Name = "ButtonSoftwareOnce"
        Me.ButtonSoftwareOnce.UseVisualStyleBackColor = True
        '
        'CheckBoxSoftware
        '
        Me.CheckBoxSoftware.AccessibleDescription = Nothing
        Me.CheckBoxSoftware.AccessibleName = Nothing
        resources.ApplyResources(Me.CheckBoxSoftware, "CheckBoxSoftware")
        Me.CheckBoxSoftware.BackgroundImage = Nothing
        Me.CheckBoxSoftware.Font = Nothing
        Me.CheckBoxSoftware.Name = "CheckBoxSoftware"
        Me.CheckBoxSoftware.UseVisualStyleBackColor = True
        '
        'ButtonSaveImage
        '
        Me.ButtonSaveImage.AccessibleDescription = Nothing
        Me.ButtonSaveImage.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonSaveImage, "ButtonSaveImage")
        Me.ButtonSaveImage.BackgroundImage = Nothing
        Me.ButtonSaveImage.Font = Nothing
        Me.ButtonSaveImage.Name = "ButtonSaveImage"
        Me.ButtonSaveImage.UseVisualStyleBackColor = True
        '
        'ButtonStopGrabbing
        '
        Me.ButtonStopGrabbing.AccessibleDescription = Nothing
        Me.ButtonStopGrabbing.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonStopGrabbing, "ButtonStopGrabbing")
        Me.ButtonStopGrabbing.BackgroundImage = Nothing
        Me.ButtonStopGrabbing.Font = Nothing
        Me.ButtonStopGrabbing.Name = "ButtonStopGrabbing"
        Me.ButtonStopGrabbing.UseVisualStyleBackColor = True
        '
        'ButtonStartGrabbing
        '
        Me.ButtonStartGrabbing.AccessibleDescription = Nothing
        Me.ButtonStartGrabbing.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonStartGrabbing, "ButtonStartGrabbing")
        Me.ButtonStartGrabbing.BackgroundImage = Nothing
        Me.ButtonStartGrabbing.Font = Nothing
        Me.ButtonStartGrabbing.Name = "ButtonStartGrabbing"
        Me.ButtonStartGrabbing.UseVisualStyleBackColor = True
        '
        'GroupBoxShowControl
        '
        Me.GroupBoxShowControl.AccessibleDescription = Nothing
        Me.GroupBoxShowControl.AccessibleName = Nothing
        resources.ApplyResources(Me.GroupBoxShowControl, "GroupBoxShowControl")
        Me.GroupBoxShowControl.BackgroundImage = Nothing
        Me.GroupBoxShowControl.Controls.Add(Me.Label9)
        Me.GroupBoxShowControl.Controls.Add(Me.Label8)
        Me.GroupBoxShowControl.Controls.Add(Me.Label7)
        Me.GroupBoxShowControl.Controls.Add(Me.Label6)
        Me.GroupBoxShowControl.Controls.Add(Me.TextBoxCamera3FrameLose)
        Me.GroupBoxShowControl.Controls.Add(Me.TextBoxCamera2FrameLose)
        Me.GroupBoxShowControl.Controls.Add(Me.TextBoxCamera3FrameGet)
        Me.GroupBoxShowControl.Controls.Add(Me.TextBoxCamera1FrameLose)
        Me.GroupBoxShowControl.Controls.Add(Me.TextBoxCamera2FrameGet)
        Me.GroupBoxShowControl.Controls.Add(Me.TextBoxCamera1FrameGet)
        Me.GroupBoxShowControl.Controls.Add(Me.TextBoxCamera0FrameLose)
        Me.GroupBoxShowControl.Controls.Add(Me.TextBoxCamera0FrameGet)
        Me.GroupBoxShowControl.Controls.Add(Me.Label5)
        Me.GroupBoxShowControl.Controls.Add(Me.Label4)
        Me.GroupBoxShowControl.Font = Nothing
        Me.GroupBoxShowControl.Name = "GroupBoxShowControl"
        Me.GroupBoxShowControl.TabStop = False
        '
        'Label9
        '
        Me.Label9.AccessibleDescription = Nothing
        Me.Label9.AccessibleName = Nothing
        resources.ApplyResources(Me.Label9, "Label9")
        Me.Label9.Font = Nothing
        Me.Label9.Name = "Label9"
        '
        'Label8
        '
        Me.Label8.AccessibleDescription = Nothing
        Me.Label8.AccessibleName = Nothing
        resources.ApplyResources(Me.Label8, "Label8")
        Me.Label8.Font = Nothing
        Me.Label8.Name = "Label8"
        '
        'Label7
        '
        Me.Label7.AccessibleDescription = Nothing
        Me.Label7.AccessibleName = Nothing
        resources.ApplyResources(Me.Label7, "Label7")
        Me.Label7.Font = Nothing
        Me.Label7.Name = "Label7"
        '
        'Label6
        '
        Me.Label6.AccessibleDescription = Nothing
        Me.Label6.AccessibleName = Nothing
        resources.ApplyResources(Me.Label6, "Label6")
        Me.Label6.Font = Nothing
        Me.Label6.Name = "Label6"
        '
        'TextBoxCamera3FrameLose
        '
        Me.TextBoxCamera3FrameLose.AccessibleDescription = Nothing
        Me.TextBoxCamera3FrameLose.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxCamera3FrameLose, "TextBoxCamera3FrameLose")
        Me.TextBoxCamera3FrameLose.BackgroundImage = Nothing
        Me.TextBoxCamera3FrameLose.Font = Nothing
        Me.TextBoxCamera3FrameLose.Name = "TextBoxCamera3FrameLose"
        Me.TextBoxCamera3FrameLose.ReadOnly = True
        '
        'TextBoxCamera2FrameLose
        '
        Me.TextBoxCamera2FrameLose.AccessibleDescription = Nothing
        Me.TextBoxCamera2FrameLose.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxCamera2FrameLose, "TextBoxCamera2FrameLose")
        Me.TextBoxCamera2FrameLose.BackgroundImage = Nothing
        Me.TextBoxCamera2FrameLose.Font = Nothing
        Me.TextBoxCamera2FrameLose.Name = "TextBoxCamera2FrameLose"
        Me.TextBoxCamera2FrameLose.ReadOnly = True
        '
        'TextBoxCamera3FrameGet
        '
        Me.TextBoxCamera3FrameGet.AccessibleDescription = Nothing
        Me.TextBoxCamera3FrameGet.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxCamera3FrameGet, "TextBoxCamera3FrameGet")
        Me.TextBoxCamera3FrameGet.BackgroundImage = Nothing
        Me.TextBoxCamera3FrameGet.Font = Nothing
        Me.TextBoxCamera3FrameGet.Name = "TextBoxCamera3FrameGet"
        Me.TextBoxCamera3FrameGet.ReadOnly = True
        '
        'TextBoxCamera1FrameLose
        '
        Me.TextBoxCamera1FrameLose.AccessibleDescription = Nothing
        Me.TextBoxCamera1FrameLose.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxCamera1FrameLose, "TextBoxCamera1FrameLose")
        Me.TextBoxCamera1FrameLose.BackgroundImage = Nothing
        Me.TextBoxCamera1FrameLose.Font = Nothing
        Me.TextBoxCamera1FrameLose.Name = "TextBoxCamera1FrameLose"
        Me.TextBoxCamera1FrameLose.ReadOnly = True
        '
        'TextBoxCamera2FrameGet
        '
        Me.TextBoxCamera2FrameGet.AccessibleDescription = Nothing
        Me.TextBoxCamera2FrameGet.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxCamera2FrameGet, "TextBoxCamera2FrameGet")
        Me.TextBoxCamera2FrameGet.BackgroundImage = Nothing
        Me.TextBoxCamera2FrameGet.Font = Nothing
        Me.TextBoxCamera2FrameGet.Name = "TextBoxCamera2FrameGet"
        Me.TextBoxCamera2FrameGet.ReadOnly = True
        '
        'TextBoxCamera1FrameGet
        '
        Me.TextBoxCamera1FrameGet.AccessibleDescription = Nothing
        Me.TextBoxCamera1FrameGet.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxCamera1FrameGet, "TextBoxCamera1FrameGet")
        Me.TextBoxCamera1FrameGet.BackgroundImage = Nothing
        Me.TextBoxCamera1FrameGet.Font = Nothing
        Me.TextBoxCamera1FrameGet.Name = "TextBoxCamera1FrameGet"
        Me.TextBoxCamera1FrameGet.ReadOnly = True
        '
        'TextBoxCamera0FrameLose
        '
        Me.TextBoxCamera0FrameLose.AccessibleDescription = Nothing
        Me.TextBoxCamera0FrameLose.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxCamera0FrameLose, "TextBoxCamera0FrameLose")
        Me.TextBoxCamera0FrameLose.BackgroundImage = Nothing
        Me.TextBoxCamera0FrameLose.Font = Nothing
        Me.TextBoxCamera0FrameLose.Name = "TextBoxCamera0FrameLose"
        Me.TextBoxCamera0FrameLose.ReadOnly = True
        '
        'TextBoxCamera0FrameGet
        '
        Me.TextBoxCamera0FrameGet.AccessibleDescription = Nothing
        Me.TextBoxCamera0FrameGet.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxCamera0FrameGet, "TextBoxCamera0FrameGet")
        Me.TextBoxCamera0FrameGet.BackgroundImage = Nothing
        Me.TextBoxCamera0FrameGet.Font = Nothing
        Me.TextBoxCamera0FrameGet.Name = "TextBoxCamera0FrameGet"
        Me.TextBoxCamera0FrameGet.ReadOnly = True
        '
        'Label5
        '
        Me.Label5.AccessibleDescription = Nothing
        Me.Label5.AccessibleName = Nothing
        resources.ApplyResources(Me.Label5, "Label5")
        Me.Label5.Font = Nothing
        Me.Label5.Name = "Label5"
        '
        'Label4
        '
        Me.Label4.AccessibleDescription = Nothing
        Me.Label4.AccessibleName = Nothing
        resources.ApplyResources(Me.Label4, "Label4")
        Me.Label4.Font = Nothing
        Me.Label4.Name = "Label4"
        '
        'TimerCameraControl
        '
        Me.TimerCameraControl.Interval = 1000
        '
        'Multiple
        '
        Me.AccessibleDescription = Nothing
        Me.AccessibleName = Nothing
        resources.ApplyResources(Me, "$this")
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackgroundImage = Nothing
        Me.Controls.Add(Me.GroupBoxShowControl)
        Me.Controls.Add(Me.GroupBoxImageControl)
        Me.Controls.Add(Me.GroupBoxSetParam)
        Me.Controls.Add(Me.GroupBoxInit)
        Me.Controls.Add(Me.GroupBoxMultiCamera)
        Me.Font = Nothing
        Me.Icon = Nothing
        Me.Name = "Multiple"
        Me.GroupBoxMultiCamera.ResumeLayout(False)
        CType(Me.PictureBoxCamera3, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PictureBoxCamera2, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PictureBoxCamera1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PictureBoxCamera0, System.ComponentModel.ISupportInitialize).EndInit()
        Me.GroupBoxInit.ResumeLayout(False)
        Me.GroupBoxInit.PerformLayout()
        Me.GroupBoxSetParam.ResumeLayout(False)
        Me.GroupBoxSetParam.PerformLayout()
        Me.GroupBoxImageControl.ResumeLayout(False)
        Me.GroupBoxImageControl.PerformLayout()
        Me.GroupBoxShowControl.ResumeLayout(False)
        Me.GroupBoxShowControl.PerformLayout()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents GroupBoxMultiCamera As System.Windows.Forms.GroupBox
    Friend WithEvents PictureBoxCamera3 As System.Windows.Forms.PictureBox
    Friend WithEvents PictureBoxCamera2 As System.Windows.Forms.PictureBox
    Friend WithEvents PictureBoxCamera1 As System.Windows.Forms.PictureBox
    Friend WithEvents PictureBoxCamera0 As System.Windows.Forms.PictureBox
    Friend WithEvents GroupBoxInit As System.Windows.Forms.GroupBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents TextBoxEnumCameraNum As System.Windows.Forms.TextBox
    Friend WithEvents ButtonInitCamera As System.Windows.Forms.Button
    Friend WithEvents TextBoxCameraUsingNum As System.Windows.Forms.TextBox
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents GroupBoxSetParam As System.Windows.Forms.GroupBox
    Friend WithEvents TextBoxGain As System.Windows.Forms.TextBox
    Friend WithEvents LabelGain As System.Windows.Forms.Label
    Friend WithEvents TextBoxExposureTime As System.Windows.Forms.TextBox
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents ButtonSetParam As System.Windows.Forms.Button
    Friend WithEvents GroupBoxImageControl As System.Windows.Forms.GroupBox
    Friend WithEvents ButtonStartGrabbing As System.Windows.Forms.Button
    Friend WithEvents ButtonSaveImage As System.Windows.Forms.Button
    Friend WithEvents CheckBoxSoftware As System.Windows.Forms.CheckBox
    Friend WithEvents ButtonSoftwareOnce As System.Windows.Forms.Button
    Friend WithEvents ButtonClose As System.Windows.Forms.Button
    Friend WithEvents GroupBoxShowControl As System.Windows.Forms.GroupBox
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents TextBoxCamera0FrameLose As System.Windows.Forms.TextBox
    Friend WithEvents TextBoxCamera0FrameGet As System.Windows.Forms.TextBox
    Friend WithEvents TextBoxCamera3FrameLose As System.Windows.Forms.TextBox
    Friend WithEvents TextBoxCamera2FrameLose As System.Windows.Forms.TextBox
    Friend WithEvents TextBoxCamera3FrameGet As System.Windows.Forms.TextBox
    Friend WithEvents TextBoxCamera1FrameLose As System.Windows.Forms.TextBox
    Friend WithEvents TextBoxCamera2FrameGet As System.Windows.Forms.TextBox
    Friend WithEvents TextBoxCamera1FrameGet As System.Windows.Forms.TextBox
    Friend WithEvents TimerCameraControl As System.Windows.Forms.Timer
    Friend WithEvents ButtonStopGrabbing As System.Windows.Forms.Button
    Friend WithEvents Label9 As System.Windows.Forms.Label
    Friend WithEvents Label8 As System.Windows.Forms.Label
    Friend WithEvents Label7 As System.Windows.Forms.Label
    Friend WithEvents Label6 As System.Windows.Forms.Label
End Class

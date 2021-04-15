<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class BasicDemo
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(BasicDemo))
        Me.ComboBoxDeviceList = New System.Windows.Forms.ComboBox
        Me.GroupBoxInit = New System.Windows.Forms.GroupBox
        Me.ButtonEnumDevice = New System.Windows.Forms.Button
        Me.ButtonCloseDevice = New System.Windows.Forms.Button
        Me.ButtonOpenDevice = New System.Windows.Forms.Button
        Me.GroupBoxGrabImage = New System.Windows.Forms.GroupBox
        Me.ButtonSoftwareOnce = New System.Windows.Forms.Button
        Me.CheckBoxSoftware = New System.Windows.Forms.CheckBox
        Me.ButtonStopGrabbing = New System.Windows.Forms.Button
        Me.ButtonStartGrabbing = New System.Windows.Forms.Button
        Me.RadioButtonTriggerOn = New System.Windows.Forms.RadioButton
        Me.RadioButtonTriggerOff = New System.Windows.Forms.RadioButton
        Me.PictureBoxDisplay = New System.Windows.Forms.PictureBox
        Me.GroupBoxImageSave = New System.Windows.Forms.GroupBox
        Me.ButtonSavePng = New System.Windows.Forms.Button
        Me.ButtonSaveTiff = New System.Windows.Forms.Button
        Me.ButtonSaveJpg = New System.Windows.Forms.Button
        Me.ButtonSaveBmp = New System.Windows.Forms.Button
        Me.GroupBoxParam = New System.Windows.Forms.GroupBox
        Me.ButtonParamSet = New System.Windows.Forms.Button
        Me.ButtonParamGet = New System.Windows.Forms.Button
        Me.TextBoxFrameRate = New System.Windows.Forms.TextBox
        Me.TextBoxGain = New System.Windows.Forms.TextBox
        Me.TextBoxExposureTime = New System.Windows.Forms.TextBox
        Me.LabelFrameRate = New System.Windows.Forms.Label
        Me.LabelGain = New System.Windows.Forms.Label
        Me.LabelExposureTime = New System.Windows.Forms.Label
        Me.GroupBoxDeviceControl = New System.Windows.Forms.GroupBox
        Me.GroupBoxInit.SuspendLayout()
        Me.GroupBoxGrabImage.SuspendLayout()
        CType(Me.PictureBoxDisplay, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.GroupBoxImageSave.SuspendLayout()
        Me.GroupBoxParam.SuspendLayout()
        Me.GroupBoxDeviceControl.SuspendLayout()
        Me.SuspendLayout()
        '
        'ComboBoxDeviceList
        '
        Me.ComboBoxDeviceList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.ComboBoxDeviceList.FormattingEnabled = True
        resources.ApplyResources(Me.ComboBoxDeviceList, "ComboBoxDeviceList")
        Me.ComboBoxDeviceList.Name = "ComboBoxDeviceList"
        '
        'GroupBoxInit
        '
        Me.GroupBoxInit.Controls.Add(Me.ButtonEnumDevice)
        resources.ApplyResources(Me.GroupBoxInit, "GroupBoxInit")
        Me.GroupBoxInit.Name = "GroupBoxInit"
        Me.GroupBoxInit.TabStop = False
        '
        'ButtonEnumDevice
        '
        resources.ApplyResources(Me.ButtonEnumDevice, "ButtonEnumDevice")
        Me.ButtonEnumDevice.Name = "ButtonEnumDevice"
        Me.ButtonEnumDevice.UseVisualStyleBackColor = True
        '
        'ButtonCloseDevice
        '
        resources.ApplyResources(Me.ButtonCloseDevice, "ButtonCloseDevice")
        Me.ButtonCloseDevice.Name = "ButtonCloseDevice"
        Me.ButtonCloseDevice.UseVisualStyleBackColor = True
        '
        'ButtonOpenDevice
        '
        resources.ApplyResources(Me.ButtonOpenDevice, "ButtonOpenDevice")
        Me.ButtonOpenDevice.Name = "ButtonOpenDevice"
        Me.ButtonOpenDevice.UseVisualStyleBackColor = True
        '
        'GroupBoxGrabImage
        '
        Me.GroupBoxGrabImage.Controls.Add(Me.ButtonSoftwareOnce)
        Me.GroupBoxGrabImage.Controls.Add(Me.CheckBoxSoftware)
        Me.GroupBoxGrabImage.Controls.Add(Me.ButtonStopGrabbing)
        Me.GroupBoxGrabImage.Controls.Add(Me.ButtonStartGrabbing)
        Me.GroupBoxGrabImage.Controls.Add(Me.RadioButtonTriggerOn)
        Me.GroupBoxGrabImage.Controls.Add(Me.RadioButtonTriggerOff)
        resources.ApplyResources(Me.GroupBoxGrabImage, "GroupBoxGrabImage")
        Me.GroupBoxGrabImage.Name = "GroupBoxGrabImage"
        Me.GroupBoxGrabImage.TabStop = False
        '
        'ButtonSoftwareOnce
        '
        resources.ApplyResources(Me.ButtonSoftwareOnce, "ButtonSoftwareOnce")
        Me.ButtonSoftwareOnce.Name = "ButtonSoftwareOnce"
        Me.ButtonSoftwareOnce.UseVisualStyleBackColor = True
        '
        'CheckBoxSoftware
        '
        resources.ApplyResources(Me.CheckBoxSoftware, "CheckBoxSoftware")
        Me.CheckBoxSoftware.Name = "CheckBoxSoftware"
        Me.CheckBoxSoftware.UseVisualStyleBackColor = True
        '
        'ButtonStopGrabbing
        '
        resources.ApplyResources(Me.ButtonStopGrabbing, "ButtonStopGrabbing")
        Me.ButtonStopGrabbing.Name = "ButtonStopGrabbing"
        Me.ButtonStopGrabbing.UseVisualStyleBackColor = True
        '
        'ButtonStartGrabbing
        '
        resources.ApplyResources(Me.ButtonStartGrabbing, "ButtonStartGrabbing")
        Me.ButtonStartGrabbing.Name = "ButtonStartGrabbing"
        Me.ButtonStartGrabbing.UseVisualStyleBackColor = True
        '
        'RadioButtonTriggerOn
        '
        resources.ApplyResources(Me.RadioButtonTriggerOn, "RadioButtonTriggerOn")
        Me.RadioButtonTriggerOn.Name = "RadioButtonTriggerOn"
        Me.RadioButtonTriggerOn.TabStop = True
        Me.RadioButtonTriggerOn.UseVisualStyleBackColor = True
        '
        'RadioButtonTriggerOff
        '
        resources.ApplyResources(Me.RadioButtonTriggerOff, "RadioButtonTriggerOff")
        Me.RadioButtonTriggerOff.Name = "RadioButtonTriggerOff"
        Me.RadioButtonTriggerOff.TabStop = True
        Me.RadioButtonTriggerOff.UseVisualStyleBackColor = True
        '
        'PictureBoxDisplay
        '
        Me.PictureBoxDisplay.BackColor = System.Drawing.SystemColors.ControlDarkDark
        resources.ApplyResources(Me.PictureBoxDisplay, "PictureBoxDisplay")
        Me.PictureBoxDisplay.Name = "PictureBoxDisplay"
        Me.PictureBoxDisplay.TabStop = False
        '
        'GroupBoxImageSave
        '
        Me.GroupBoxImageSave.Controls.Add(Me.ButtonSavePng)
        Me.GroupBoxImageSave.Controls.Add(Me.ButtonSaveTiff)
        Me.GroupBoxImageSave.Controls.Add(Me.ButtonSaveJpg)
        Me.GroupBoxImageSave.Controls.Add(Me.ButtonSaveBmp)
        resources.ApplyResources(Me.GroupBoxImageSave, "GroupBoxImageSave")
        Me.GroupBoxImageSave.Name = "GroupBoxImageSave"
        Me.GroupBoxImageSave.TabStop = False
        '
        'ButtonSavePng
        '
        resources.ApplyResources(Me.ButtonSavePng, "ButtonSavePng")
        Me.ButtonSavePng.Name = "ButtonSavePng"
        Me.ButtonSavePng.UseVisualStyleBackColor = True
        '
        'ButtonSaveTiff
        '
        resources.ApplyResources(Me.ButtonSaveTiff, "ButtonSaveTiff")
        Me.ButtonSaveTiff.Name = "ButtonSaveTiff"
        Me.ButtonSaveTiff.UseVisualStyleBackColor = True
        '
        'ButtonSaveJpg
        '
        resources.ApplyResources(Me.ButtonSaveJpg, "ButtonSaveJpg")
        Me.ButtonSaveJpg.Name = "ButtonSaveJpg"
        Me.ButtonSaveJpg.UseVisualStyleBackColor = True
        '
        'ButtonSaveBmp
        '
        resources.ApplyResources(Me.ButtonSaveBmp, "ButtonSaveBmp")
        Me.ButtonSaveBmp.Name = "ButtonSaveBmp"
        Me.ButtonSaveBmp.UseVisualStyleBackColor = True
        '
        'GroupBoxParam
        '
        Me.GroupBoxParam.Controls.Add(Me.ButtonParamSet)
        Me.GroupBoxParam.Controls.Add(Me.ButtonParamGet)
        Me.GroupBoxParam.Controls.Add(Me.TextBoxFrameRate)
        Me.GroupBoxParam.Controls.Add(Me.TextBoxGain)
        Me.GroupBoxParam.Controls.Add(Me.TextBoxExposureTime)
        Me.GroupBoxParam.Controls.Add(Me.LabelFrameRate)
        Me.GroupBoxParam.Controls.Add(Me.LabelGain)
        Me.GroupBoxParam.Controls.Add(Me.LabelExposureTime)
        resources.ApplyResources(Me.GroupBoxParam, "GroupBoxParam")
        Me.GroupBoxParam.Name = "GroupBoxParam"
        Me.GroupBoxParam.TabStop = False
        '
        'ButtonParamSet
        '
        resources.ApplyResources(Me.ButtonParamSet, "ButtonParamSet")
        Me.ButtonParamSet.Name = "ButtonParamSet"
        Me.ButtonParamSet.UseVisualStyleBackColor = True
        '
        'ButtonParamGet
        '
        resources.ApplyResources(Me.ButtonParamGet, "ButtonParamGet")
        Me.ButtonParamGet.Name = "ButtonParamGet"
        Me.ButtonParamGet.UseVisualStyleBackColor = True
        '
        'TextBoxFrameRate
        '
        resources.ApplyResources(Me.TextBoxFrameRate, "TextBoxFrameRate")
        Me.TextBoxFrameRate.Name = "TextBoxFrameRate"
        '
        'TextBoxGain
        '
        resources.ApplyResources(Me.TextBoxGain, "TextBoxGain")
        Me.TextBoxGain.Name = "TextBoxGain"
        '
        'TextBoxExposureTime
        '
        resources.ApplyResources(Me.TextBoxExposureTime, "TextBoxExposureTime")
        Me.TextBoxExposureTime.Name = "TextBoxExposureTime"
        '
        'LabelFrameRate
        '
        resources.ApplyResources(Me.LabelFrameRate, "LabelFrameRate")
        Me.LabelFrameRate.Name = "LabelFrameRate"
        '
        'LabelGain
        '
        resources.ApplyResources(Me.LabelGain, "LabelGain")
        Me.LabelGain.Name = "LabelGain"
        '
        'LabelExposureTime
        '
        resources.ApplyResources(Me.LabelExposureTime, "LabelExposureTime")
        Me.LabelExposureTime.Name = "LabelExposureTime"
        '
        'GroupBoxDeviceControl
        '
        Me.GroupBoxDeviceControl.Controls.Add(Me.ButtonOpenDevice)
        Me.GroupBoxDeviceControl.Controls.Add(Me.ButtonCloseDevice)
        resources.ApplyResources(Me.GroupBoxDeviceControl, "GroupBoxDeviceControl")
        Me.GroupBoxDeviceControl.Name = "GroupBoxDeviceControl"
        Me.GroupBoxDeviceControl.TabStop = False
        '
        'BasicDemo
        '
        resources.ApplyResources(Me, "$this")
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.Controls.Add(Me.GroupBoxDeviceControl)
        Me.Controls.Add(Me.GroupBoxParam)
        Me.Controls.Add(Me.GroupBoxImageSave)
        Me.Controls.Add(Me.PictureBoxDisplay)
        Me.Controls.Add(Me.GroupBoxGrabImage)
        Me.Controls.Add(Me.GroupBoxInit)
        Me.Controls.Add(Me.ComboBoxDeviceList)
        Me.Name = "BasicDemo"
        Me.GroupBoxInit.ResumeLayout(False)
        Me.GroupBoxGrabImage.ResumeLayout(False)
        Me.GroupBoxGrabImage.PerformLayout()
        CType(Me.PictureBoxDisplay, System.ComponentModel.ISupportInitialize).EndInit()
        Me.GroupBoxImageSave.ResumeLayout(False)
        Me.GroupBoxParam.ResumeLayout(False)
        Me.GroupBoxParam.PerformLayout()
        Me.GroupBoxDeviceControl.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents ComboBoxDeviceList As System.Windows.Forms.ComboBox
    Friend WithEvents GroupBoxInit As System.Windows.Forms.GroupBox
    Friend WithEvents ButtonEnumDevice As System.Windows.Forms.Button
    Friend WithEvents ButtonOpenDevice As System.Windows.Forms.Button
    Friend WithEvents ButtonCloseDevice As System.Windows.Forms.Button
    Friend WithEvents GroupBoxGrabImage As System.Windows.Forms.GroupBox
    Friend WithEvents RadioButtonTriggerOn As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButtonTriggerOff As System.Windows.Forms.RadioButton
    Friend WithEvents ButtonStartGrabbing As System.Windows.Forms.Button
    Friend WithEvents ButtonStopGrabbing As System.Windows.Forms.Button
    Friend WithEvents PictureBoxDisplay As System.Windows.Forms.PictureBox
    Friend WithEvents CheckBoxSoftware As System.Windows.Forms.CheckBox
    Friend WithEvents ButtonSoftwareOnce As System.Windows.Forms.Button
    Friend WithEvents GroupBoxImageSave As System.Windows.Forms.GroupBox
    Friend WithEvents ButtonSaveBmp As System.Windows.Forms.Button
    Friend WithEvents ButtonSaveJpg As System.Windows.Forms.Button
    Friend WithEvents GroupBoxParam As System.Windows.Forms.GroupBox
    Friend WithEvents LabelGain As System.Windows.Forms.Label
    Friend WithEvents LabelExposureTime As System.Windows.Forms.Label
    Friend WithEvents TextBoxExposureTime As System.Windows.Forms.TextBox
    Friend WithEvents LabelFrameRate As System.Windows.Forms.Label
    Friend WithEvents ButtonParamSet As System.Windows.Forms.Button
    Friend WithEvents ButtonParamGet As System.Windows.Forms.Button
    Friend WithEvents TextBoxFrameRate As System.Windows.Forms.TextBox
    Friend WithEvents TextBoxGain As System.Windows.Forms.TextBox
    Friend WithEvents GroupBoxDeviceControl As System.Windows.Forms.GroupBox
    Friend WithEvents ButtonSavePng As System.Windows.Forms.Button
    Friend WithEvents ButtonSaveTiff As System.Windows.Forms.Button
End Class

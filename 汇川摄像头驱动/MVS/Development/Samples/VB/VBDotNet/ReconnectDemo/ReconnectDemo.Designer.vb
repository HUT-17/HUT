<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class ReconnectDemo
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(ReconnectDemo))
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
        Me.GroupBoxDeviceControl = New System.Windows.Forms.GroupBox
        Me.GroupBoxInit.SuspendLayout()
        Me.GroupBoxGrabImage.SuspendLayout()
        CType(Me.PictureBoxDisplay, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.GroupBoxDeviceControl.SuspendLayout()
        Me.SuspendLayout()
        '
        'ComboBoxDeviceList
        '
        Me.ComboBoxDeviceList.AccessibleDescription = Nothing
        Me.ComboBoxDeviceList.AccessibleName = Nothing
        resources.ApplyResources(Me.ComboBoxDeviceList, "ComboBoxDeviceList")
        Me.ComboBoxDeviceList.BackgroundImage = Nothing
        Me.ComboBoxDeviceList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.ComboBoxDeviceList.Font = Nothing
        Me.ComboBoxDeviceList.FormattingEnabled = True
        Me.ComboBoxDeviceList.Name = "ComboBoxDeviceList"
        '
        'GroupBoxInit
        '
        Me.GroupBoxInit.AccessibleDescription = Nothing
        Me.GroupBoxInit.AccessibleName = Nothing
        resources.ApplyResources(Me.GroupBoxInit, "GroupBoxInit")
        Me.GroupBoxInit.BackgroundImage = Nothing
        Me.GroupBoxInit.Controls.Add(Me.ButtonEnumDevice)
        Me.GroupBoxInit.Font = Nothing
        Me.GroupBoxInit.Name = "GroupBoxInit"
        Me.GroupBoxInit.TabStop = False
        '
        'ButtonEnumDevice
        '
        Me.ButtonEnumDevice.AccessibleDescription = Nothing
        Me.ButtonEnumDevice.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonEnumDevice, "ButtonEnumDevice")
        Me.ButtonEnumDevice.BackgroundImage = Nothing
        Me.ButtonEnumDevice.Font = Nothing
        Me.ButtonEnumDevice.Name = "ButtonEnumDevice"
        Me.ButtonEnumDevice.UseVisualStyleBackColor = True
        '
        'ButtonCloseDevice
        '
        Me.ButtonCloseDevice.AccessibleDescription = Nothing
        Me.ButtonCloseDevice.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonCloseDevice, "ButtonCloseDevice")
        Me.ButtonCloseDevice.BackgroundImage = Nothing
        Me.ButtonCloseDevice.Font = Nothing
        Me.ButtonCloseDevice.Name = "ButtonCloseDevice"
        Me.ButtonCloseDevice.UseVisualStyleBackColor = True
        '
        'ButtonOpenDevice
        '
        Me.ButtonOpenDevice.AccessibleDescription = Nothing
        Me.ButtonOpenDevice.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonOpenDevice, "ButtonOpenDevice")
        Me.ButtonOpenDevice.BackgroundImage = Nothing
        Me.ButtonOpenDevice.Font = Nothing
        Me.ButtonOpenDevice.Name = "ButtonOpenDevice"
        Me.ButtonOpenDevice.UseVisualStyleBackColor = True
        '
        'GroupBoxGrabImage
        '
        Me.GroupBoxGrabImage.AccessibleDescription = Nothing
        Me.GroupBoxGrabImage.AccessibleName = Nothing
        resources.ApplyResources(Me.GroupBoxGrabImage, "GroupBoxGrabImage")
        Me.GroupBoxGrabImage.BackgroundImage = Nothing
        Me.GroupBoxGrabImage.Controls.Add(Me.ButtonSoftwareOnce)
        Me.GroupBoxGrabImage.Controls.Add(Me.CheckBoxSoftware)
        Me.GroupBoxGrabImage.Controls.Add(Me.ButtonStopGrabbing)
        Me.GroupBoxGrabImage.Controls.Add(Me.ButtonStartGrabbing)
        Me.GroupBoxGrabImage.Controls.Add(Me.RadioButtonTriggerOn)
        Me.GroupBoxGrabImage.Controls.Add(Me.RadioButtonTriggerOff)
        Me.GroupBoxGrabImage.Font = Nothing
        Me.GroupBoxGrabImage.Name = "GroupBoxGrabImage"
        Me.GroupBoxGrabImage.TabStop = False
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
        'RadioButtonTriggerOn
        '
        Me.RadioButtonTriggerOn.AccessibleDescription = Nothing
        Me.RadioButtonTriggerOn.AccessibleName = Nothing
        resources.ApplyResources(Me.RadioButtonTriggerOn, "RadioButtonTriggerOn")
        Me.RadioButtonTriggerOn.BackgroundImage = Nothing
        Me.RadioButtonTriggerOn.Font = Nothing
        Me.RadioButtonTriggerOn.Name = "RadioButtonTriggerOn"
        Me.RadioButtonTriggerOn.TabStop = True
        Me.RadioButtonTriggerOn.UseVisualStyleBackColor = True
        '
        'RadioButtonTriggerOff
        '
        Me.RadioButtonTriggerOff.AccessibleDescription = Nothing
        Me.RadioButtonTriggerOff.AccessibleName = Nothing
        resources.ApplyResources(Me.RadioButtonTriggerOff, "RadioButtonTriggerOff")
        Me.RadioButtonTriggerOff.BackgroundImage = Nothing
        Me.RadioButtonTriggerOff.Font = Nothing
        Me.RadioButtonTriggerOff.Name = "RadioButtonTriggerOff"
        Me.RadioButtonTriggerOff.TabStop = True
        Me.RadioButtonTriggerOff.UseVisualStyleBackColor = True
        '
        'PictureBoxDisplay
        '
        Me.PictureBoxDisplay.AccessibleDescription = Nothing
        Me.PictureBoxDisplay.AccessibleName = Nothing
        resources.ApplyResources(Me.PictureBoxDisplay, "PictureBoxDisplay")
        Me.PictureBoxDisplay.BackColor = System.Drawing.SystemColors.ControlDarkDark
        Me.PictureBoxDisplay.BackgroundImage = Nothing
        Me.PictureBoxDisplay.Font = Nothing
        Me.PictureBoxDisplay.ImageLocation = Nothing
        Me.PictureBoxDisplay.Name = "PictureBoxDisplay"
        Me.PictureBoxDisplay.TabStop = False
        '
        'GroupBoxDeviceControl
        '
        Me.GroupBoxDeviceControl.AccessibleDescription = Nothing
        Me.GroupBoxDeviceControl.AccessibleName = Nothing
        resources.ApplyResources(Me.GroupBoxDeviceControl, "GroupBoxDeviceControl")
        Me.GroupBoxDeviceControl.BackgroundImage = Nothing
        Me.GroupBoxDeviceControl.Controls.Add(Me.ButtonOpenDevice)
        Me.GroupBoxDeviceControl.Controls.Add(Me.ButtonCloseDevice)
        Me.GroupBoxDeviceControl.Font = Nothing
        Me.GroupBoxDeviceControl.Name = "GroupBoxDeviceControl"
        Me.GroupBoxDeviceControl.TabStop = False
        '
        'ReconnectDemo
        '
        Me.AccessibleDescription = Nothing
        Me.AccessibleName = Nothing
        resources.ApplyResources(Me, "$this")
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackgroundImage = Nothing
        Me.Controls.Add(Me.GroupBoxDeviceControl)
        Me.Controls.Add(Me.PictureBoxDisplay)
        Me.Controls.Add(Me.GroupBoxGrabImage)
        Me.Controls.Add(Me.GroupBoxInit)
        Me.Controls.Add(Me.ComboBoxDeviceList)
        Me.Font = Nothing
        Me.Icon = Nothing
        Me.Name = "ReconnectDemo"
        Me.GroupBoxInit.ResumeLayout(False)
        Me.GroupBoxGrabImage.ResumeLayout(False)
        Me.GroupBoxGrabImage.PerformLayout()
        CType(Me.PictureBoxDisplay, System.ComponentModel.ISupportInitialize).EndInit()
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
    Friend WithEvents GroupBoxDeviceControl As System.Windows.Forms.GroupBox
End Class

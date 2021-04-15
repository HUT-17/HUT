<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class SetIO
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(SetIO))
        Me.ButtonCloseDevice = New System.Windows.Forms.Button
        Me.GroupBoxInit = New System.Windows.Forms.GroupBox
        Me.ButtonOpenDevice = New System.Windows.Forms.Button
        Me.ButtonEnumDevice = New System.Windows.Forms.Button
        Me.ComboBoxDeviceList = New System.Windows.Forms.ComboBox
        Me.GroupBoxSetIO = New System.Windows.Forms.GroupBox
        Me.ButtonSetMode = New System.Windows.Forms.Button
        Me.ButtonGetMode = New System.Windows.Forms.Button
        Me.ButtonSetSelector = New System.Windows.Forms.Button
        Me.ButtonGetSelector = New System.Windows.Forms.Button
        Me.ComboBoxLineMode = New System.Windows.Forms.ComboBox
        Me.ComboBoxLineSelector = New System.Windows.Forms.ComboBox
        Me.LabelLineMode = New System.Windows.Forms.Label
        Me.LabelLineSelector = New System.Windows.Forms.Label
        Me.GroupBoxInit.SuspendLayout()
        Me.GroupBoxSetIO.SuspendLayout()
        Me.SuspendLayout()
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
        'GroupBoxInit
        '
        Me.GroupBoxInit.AccessibleDescription = Nothing
        Me.GroupBoxInit.AccessibleName = Nothing
        resources.ApplyResources(Me.GroupBoxInit, "GroupBoxInit")
        Me.GroupBoxInit.BackgroundImage = Nothing
        Me.GroupBoxInit.Controls.Add(Me.ButtonCloseDevice)
        Me.GroupBoxInit.Controls.Add(Me.ButtonOpenDevice)
        Me.GroupBoxInit.Controls.Add(Me.ButtonEnumDevice)
        Me.GroupBoxInit.Font = Nothing
        Me.GroupBoxInit.Name = "GroupBoxInit"
        Me.GroupBoxInit.TabStop = False
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
        'GroupBoxSetIO
        '
        Me.GroupBoxSetIO.AccessibleDescription = Nothing
        Me.GroupBoxSetIO.AccessibleName = Nothing
        resources.ApplyResources(Me.GroupBoxSetIO, "GroupBoxSetIO")
        Me.GroupBoxSetIO.BackgroundImage = Nothing
        Me.GroupBoxSetIO.Controls.Add(Me.ButtonSetMode)
        Me.GroupBoxSetIO.Controls.Add(Me.ButtonGetMode)
        Me.GroupBoxSetIO.Controls.Add(Me.ButtonSetSelector)
        Me.GroupBoxSetIO.Controls.Add(Me.ButtonGetSelector)
        Me.GroupBoxSetIO.Controls.Add(Me.ComboBoxLineMode)
        Me.GroupBoxSetIO.Controls.Add(Me.ComboBoxLineSelector)
        Me.GroupBoxSetIO.Controls.Add(Me.LabelLineMode)
        Me.GroupBoxSetIO.Controls.Add(Me.LabelLineSelector)
        Me.GroupBoxSetIO.Font = Nothing
        Me.GroupBoxSetIO.Name = "GroupBoxSetIO"
        Me.GroupBoxSetIO.TabStop = False
        '
        'ButtonSetMode
        '
        Me.ButtonSetMode.AccessibleDescription = Nothing
        Me.ButtonSetMode.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonSetMode, "ButtonSetMode")
        Me.ButtonSetMode.BackgroundImage = Nothing
        Me.ButtonSetMode.Font = Nothing
        Me.ButtonSetMode.Name = "ButtonSetMode"
        Me.ButtonSetMode.UseVisualStyleBackColor = True
        '
        'ButtonGetMode
        '
        Me.ButtonGetMode.AccessibleDescription = Nothing
        Me.ButtonGetMode.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonGetMode, "ButtonGetMode")
        Me.ButtonGetMode.BackgroundImage = Nothing
        Me.ButtonGetMode.Font = Nothing
        Me.ButtonGetMode.Name = "ButtonGetMode"
        Me.ButtonGetMode.UseVisualStyleBackColor = True
        '
        'ButtonSetSelector
        '
        Me.ButtonSetSelector.AccessibleDescription = Nothing
        Me.ButtonSetSelector.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonSetSelector, "ButtonSetSelector")
        Me.ButtonSetSelector.BackgroundImage = Nothing
        Me.ButtonSetSelector.Font = Nothing
        Me.ButtonSetSelector.Name = "ButtonSetSelector"
        Me.ButtonSetSelector.UseVisualStyleBackColor = True
        '
        'ButtonGetSelector
        '
        Me.ButtonGetSelector.AccessibleDescription = Nothing
        Me.ButtonGetSelector.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonGetSelector, "ButtonGetSelector")
        Me.ButtonGetSelector.BackgroundImage = Nothing
        Me.ButtonGetSelector.Font = Nothing
        Me.ButtonGetSelector.Name = "ButtonGetSelector"
        Me.ButtonGetSelector.UseVisualStyleBackColor = True
        '
        'ComboBoxLineMode
        '
        Me.ComboBoxLineMode.AccessibleDescription = Nothing
        Me.ComboBoxLineMode.AccessibleName = Nothing
        resources.ApplyResources(Me.ComboBoxLineMode, "ComboBoxLineMode")
        Me.ComboBoxLineMode.BackgroundImage = Nothing
        Me.ComboBoxLineMode.Font = Nothing
        Me.ComboBoxLineMode.FormattingEnabled = True
        Me.ComboBoxLineMode.Name = "ComboBoxLineMode"
        '
        'ComboBoxLineSelector
        '
        Me.ComboBoxLineSelector.AccessibleDescription = Nothing
        Me.ComboBoxLineSelector.AccessibleName = Nothing
        resources.ApplyResources(Me.ComboBoxLineSelector, "ComboBoxLineSelector")
        Me.ComboBoxLineSelector.BackgroundImage = Nothing
        Me.ComboBoxLineSelector.Font = Nothing
        Me.ComboBoxLineSelector.FormattingEnabled = True
        Me.ComboBoxLineSelector.Name = "ComboBoxLineSelector"
        '
        'LabelLineMode
        '
        Me.LabelLineMode.AccessibleDescription = Nothing
        Me.LabelLineMode.AccessibleName = Nothing
        resources.ApplyResources(Me.LabelLineMode, "LabelLineMode")
        Me.LabelLineMode.Font = Nothing
        Me.LabelLineMode.Name = "LabelLineMode"
        '
        'LabelLineSelector
        '
        Me.LabelLineSelector.AccessibleDescription = Nothing
        Me.LabelLineSelector.AccessibleName = Nothing
        resources.ApplyResources(Me.LabelLineSelector, "LabelLineSelector")
        Me.LabelLineSelector.Font = Nothing
        Me.LabelLineSelector.Name = "LabelLineSelector"
        '
        'SetIO
        '
        Me.AccessibleDescription = Nothing
        Me.AccessibleName = Nothing
        resources.ApplyResources(Me, "$this")
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackgroundImage = Nothing
        Me.Controls.Add(Me.GroupBoxSetIO)
        Me.Controls.Add(Me.GroupBoxInit)
        Me.Controls.Add(Me.ComboBoxDeviceList)
        Me.Font = Nothing
        Me.Icon = Nothing
        Me.Name = "SetIO"
        Me.GroupBoxInit.ResumeLayout(False)
        Me.GroupBoxSetIO.ResumeLayout(False)
        Me.GroupBoxSetIO.PerformLayout()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents ButtonCloseDevice As System.Windows.Forms.Button
    Friend WithEvents GroupBoxInit As System.Windows.Forms.GroupBox
    Friend WithEvents ButtonOpenDevice As System.Windows.Forms.Button
    Friend WithEvents ButtonEnumDevice As System.Windows.Forms.Button
    Friend WithEvents ComboBoxDeviceList As System.Windows.Forms.ComboBox
    Friend WithEvents GroupBoxSetIO As System.Windows.Forms.GroupBox
    Friend WithEvents ComboBoxLineMode As System.Windows.Forms.ComboBox
    Friend WithEvents ComboBoxLineSelector As System.Windows.Forms.ComboBox
    Friend WithEvents LabelLineMode As System.Windows.Forms.Label
    Friend WithEvents LabelLineSelector As System.Windows.Forms.Label
    Friend WithEvents ButtonSetMode As System.Windows.Forms.Button
    Friend WithEvents ButtonGetMode As System.Windows.Forms.Button
    Friend WithEvents ButtonSetSelector As System.Windows.Forms.Button
    Friend WithEvents ButtonGetSelector As System.Windows.Forms.Button
End Class

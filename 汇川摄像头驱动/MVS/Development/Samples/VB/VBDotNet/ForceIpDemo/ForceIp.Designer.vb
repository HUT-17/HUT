<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class ForceIp
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(ForceIp))
        Me.GroupBoxInit = New System.Windows.Forms.GroupBox
        Me.ButtonEnumDevice = New System.Windows.Forms.Button
        Me.ComboBoxDeviceList = New System.Windows.Forms.ComboBox
        Me.GroupBoxSetIp = New System.Windows.Forms.GroupBox
        Me.Label3 = New System.Windows.Forms.Label
        Me.Label2 = New System.Windows.Forms.Label
        Me.Label1 = New System.Windows.Forms.Label
        Me.TextBoxSetDefaultWay = New System.Windows.Forms.TextBox
        Me.TextBoxSetMask = New System.Windows.Forms.TextBox
        Me.ButtonSetIp = New System.Windows.Forms.Button
        Me.TextBoxSetIp = New System.Windows.Forms.TextBox
        Me.LabelIpInfo = New System.Windows.Forms.Label
        Me.GroupBoxInit.SuspendLayout()
        Me.GroupBoxSetIp.SuspendLayout()
        Me.SuspendLayout()
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
        'GroupBoxSetIp
        '
        Me.GroupBoxSetIp.AccessibleDescription = Nothing
        Me.GroupBoxSetIp.AccessibleName = Nothing
        resources.ApplyResources(Me.GroupBoxSetIp, "GroupBoxSetIp")
        Me.GroupBoxSetIp.BackgroundImage = Nothing
        Me.GroupBoxSetIp.Controls.Add(Me.Label3)
        Me.GroupBoxSetIp.Controls.Add(Me.Label2)
        Me.GroupBoxSetIp.Controls.Add(Me.Label1)
        Me.GroupBoxSetIp.Controls.Add(Me.TextBoxSetDefaultWay)
        Me.GroupBoxSetIp.Controls.Add(Me.TextBoxSetMask)
        Me.GroupBoxSetIp.Controls.Add(Me.ButtonSetIp)
        Me.GroupBoxSetIp.Controls.Add(Me.TextBoxSetIp)
        Me.GroupBoxSetIp.Controls.Add(Me.LabelIpInfo)
        Me.GroupBoxSetIp.Font = Nothing
        Me.GroupBoxSetIp.Name = "GroupBoxSetIp"
        Me.GroupBoxSetIp.TabStop = False
        '
        'Label3
        '
        Me.Label3.AccessibleDescription = Nothing
        Me.Label3.AccessibleName = Nothing
        resources.ApplyResources(Me.Label3, "Label3")
        Me.Label3.Font = Nothing
        Me.Label3.Name = "Label3"
        '
        'Label2
        '
        Me.Label2.AccessibleDescription = Nothing
        Me.Label2.AccessibleName = Nothing
        resources.ApplyResources(Me.Label2, "Label2")
        Me.Label2.Font = Nothing
        Me.Label2.Name = "Label2"
        '
        'Label1
        '
        Me.Label1.AccessibleDescription = Nothing
        Me.Label1.AccessibleName = Nothing
        resources.ApplyResources(Me.Label1, "Label1")
        Me.Label1.Font = Nothing
        Me.Label1.Name = "Label1"
        '
        'TextBoxSetDefaultWay
        '
        Me.TextBoxSetDefaultWay.AccessibleDescription = Nothing
        Me.TextBoxSetDefaultWay.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxSetDefaultWay, "TextBoxSetDefaultWay")
        Me.TextBoxSetDefaultWay.BackgroundImage = Nothing
        Me.TextBoxSetDefaultWay.Font = Nothing
        Me.TextBoxSetDefaultWay.Name = "TextBoxSetDefaultWay"
        '
        'TextBoxSetMask
        '
        Me.TextBoxSetMask.AccessibleDescription = Nothing
        Me.TextBoxSetMask.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxSetMask, "TextBoxSetMask")
        Me.TextBoxSetMask.BackgroundImage = Nothing
        Me.TextBoxSetMask.Font = Nothing
        Me.TextBoxSetMask.Name = "TextBoxSetMask"
        '
        'ButtonSetIp
        '
        Me.ButtonSetIp.AccessibleDescription = Nothing
        Me.ButtonSetIp.AccessibleName = Nothing
        resources.ApplyResources(Me.ButtonSetIp, "ButtonSetIp")
        Me.ButtonSetIp.BackgroundImage = Nothing
        Me.ButtonSetIp.Font = Nothing
        Me.ButtonSetIp.Name = "ButtonSetIp"
        Me.ButtonSetIp.UseVisualStyleBackColor = True
        '
        'TextBoxSetIp
        '
        Me.TextBoxSetIp.AccessibleDescription = Nothing
        Me.TextBoxSetIp.AccessibleName = Nothing
        resources.ApplyResources(Me.TextBoxSetIp, "TextBoxSetIp")
        Me.TextBoxSetIp.BackgroundImage = Nothing
        Me.TextBoxSetIp.Font = Nothing
        Me.TextBoxSetIp.Name = "TextBoxSetIp"
        '
        'LabelIpInfo
        '
        Me.LabelIpInfo.AccessibleDescription = Nothing
        Me.LabelIpInfo.AccessibleName = Nothing
        resources.ApplyResources(Me.LabelIpInfo, "LabelIpInfo")
        Me.LabelIpInfo.Font = Nothing
        Me.LabelIpInfo.Name = "LabelIpInfo"
        '
        'ForceIp
        '
        Me.AccessibleDescription = Nothing
        Me.AccessibleName = Nothing
        resources.ApplyResources(Me, "$this")
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackgroundImage = Nothing
        Me.Controls.Add(Me.GroupBoxSetIp)
        Me.Controls.Add(Me.GroupBoxInit)
        Me.Controls.Add(Me.ComboBoxDeviceList)
        Me.Font = Nothing
        Me.Icon = Nothing
        Me.Name = "ForceIp"
        Me.GroupBoxInit.ResumeLayout(False)
        Me.GroupBoxSetIp.ResumeLayout(False)
        Me.GroupBoxSetIp.PerformLayout()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents GroupBoxInit As System.Windows.Forms.GroupBox
    Friend WithEvents ButtonEnumDevice As System.Windows.Forms.Button
    Friend WithEvents ComboBoxDeviceList As System.Windows.Forms.ComboBox
    Friend WithEvents GroupBoxSetIp As System.Windows.Forms.GroupBox
    Friend WithEvents ButtonSetIp As System.Windows.Forms.Button
    Friend WithEvents TextBoxSetIp As System.Windows.Forms.TextBox
    Friend WithEvents LabelIpInfo As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents TextBoxSetDefaultWay As System.Windows.Forms.TextBox
    Friend WithEvents TextBoxSetMask As System.Windows.Forms.TextBox
End Class

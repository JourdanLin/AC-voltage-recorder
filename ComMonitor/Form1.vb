Imports System.IO.Ports
Imports System.Threading

Public Class Form1
    Dim myPort As SerialPort
    Dim receivingThread As Thread

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        ' 列出可用的COM端口
        For Each portName As String In SerialPort.GetPortNames()
            ComboBox1.Items.Add(portName)
        Next
    End Sub

    Private Sub ComboBox1_SelectedIndexChanged(sender As Object, e As EventArgs) Handles ComboBox1.SelectedIndexChanged
        ' 選擇COM端口後打開串口
        Dim selectedPort As String = ComboBox1.SelectedItem.ToString()
        myPort = New SerialPort(selectedPort, 9600)
        myPort.Open()
        ' 啟動線程來接收串口數據
        receivingThread = New Thread(AddressOf ReceiveData)
        receivingThread.IsBackground = True
        receivingThread.Start()
    End Sub

    Private Sub ReceiveData()
        While True
            Try
                ' 讀取串口數據並顯示在TextBox2中
                Dim receivedData As String = myPort.ReadLine()
                Me.Invoke(Sub() TextBox1.AppendText(receivedData & vbCrLf))
            Catch ex As Exception
                ' 如果發生錯誤，退出循環
                Exit While
            End Try
        End While
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        ' 向Arduino發送更改時間的命令
        Dim currentTime As String = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
        myPort.WriteLine(currentTime)
        ' 同時在TextBox1中顯示當前時間
        TextBox1.Text = currentTime
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        ' 關閉串口
        If myPort IsNot Nothing AndAlso myPort.IsOpen Then
            myPort.Close()
        End If
        TextBox1.Text += "----Stop Port ---" & vbCrLf
        ' 停止接收數據的線程
        If receivingThread IsNot Nothing AndAlso receivingThread.IsAlive Then
            receivingThread.Abort()
        End If
    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        myPort.WriteLine("LIST_FILES")
        'TextBox2.Invoke(Sub() TextBox2.Text = receivedData.Substring(6))
    End Sub

End Class

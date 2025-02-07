using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ExtendedSerialPort_NS;
using System.IO.Ports;
using System.Printing;
using System.Windows.Threading;



namespace RobotInterface;
/// <summary>
/// Interaction logic for MainWindow.xaml
/// </summary>
public partial class MainWindow : Window
{
    
    ExtendedSerialPort serialPort1;
    DispatcherTimer timerAffichage;
    /// <summary>
    /// Init timer affichage
    /// </summary>
    Robot robot = new Robot();

    public MainWindow()
    {
        InitializeComponent();
        serialPort1 = new ExtendedSerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
        serialPort1.Open();
        serialPort1.DataReceived += SerialPort1_DataReceived;

        timerAffichage = new DispatcherTimer();
        timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
        timerAffichage.Tick += TimerAffichage_Tick;
        timerAffichage.Start();

    }

    private void TimerAffichage_Tick(object? sender, EventArgs e)
    {
        while (robot.byteListReceived.Count > 0)
        {
            var v = robot.byteListReceived.Dequeue();
            DecodeMessage(v);
            //textBoxReception.Text += v.ToString("X2")+ " ";
        }
        //robot.receivedText = "";
    }

    void SendMessage()
    {
        //textBoxReception.Text += ("Reçu :" + textBoxEmission.Text + "\n");
        serialPort1.WriteLine(textBoxEmission.Text);
        textBoxEmission.Text = null;      
        
    }
    void Clear()
    {
        textBoxReception.Text = null;
    }
    void Test()
    {
        byte[] byteList = new byte[20];
        for (int i = 0; i < 20; i++)
        {
            byteList[i] = (byte)(2 * i);
        }
        serialPort1.Write(byteList, 0, byteList.Length);
    }

    public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
    {
        //robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
        foreach (var item in e.Data) 
        { 
            robot.byteListReceived.Enqueue(item); 
        }
        
    }








    private void TextBoxEmission_KeyUp(object sender, KeyEventArgs e)
    {
        if (e.Key == Key.Enter)
        {
            SendMessage();
        }
    }

    private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
    {

        SendMessage();
        if (buttonEnvoyer.Background == Brushes.Beige)
        {
            buttonEnvoyer.Background = Brushes.RoyalBlue;
        }
        else
        {
            buttonEnvoyer.Background = Brushes.Beige;
        }
    }

    private void buttonClear_Click(object sender, RoutedEventArgs e)
    {

        Clear();
    }

    private void buttonTest_Click(object sender, RoutedEventArgs e)
    {
        //Test();
        string s = "Bonjour";
        byte[] array = Encoding.ASCII.GetBytes(s);
        UartEncodeAndSendMessage(0x0080, array.Length, array);
    }

    public enum StateReception
    {
        Waiting,
        FunctionMSB,
        FunctionLSB,
        PayloadLengthMSB,
        PayloadLengthLSB,
        Payload,
        CheckSum
    }
    StateReception rcvState = StateReception.Waiting;
    int msgDecodedFunction = 0;
    int msgDecodedPayloadLength = 0;
    byte[] msgDecodedPayload;
    int msgDecodedPayloadIndex = 0;
    private void DecodeMessage(byte c)
    {
        int pos = 0;
        switch (rcvState)
        {
            case StateReception.Waiting:
                if(c== 0xFE)
                    rcvState = StateReception.FunctionMSB;
            break;
            case StateReception.FunctionMSB:
                msgDecodedFunction = c << 8;
                rcvState = StateReception.FunctionLSB;
                break;
            case StateReception.FunctionLSB:
                msgDecodedFunction |= c;
                rcvState = StateReception.PayloadLengthMSB;
                break;
            case StateReception.PayloadLengthMSB:
                msgDecodedFunction = c << 8;
                rcvState = StateReception.PayloadLengthMSB;
                break;
            case StateReception.PayloadLengthLSB:
                msgDecodedPayloadLength |= c;
                if (msgDecodedPayloadLength == 0)
                    rcvState = StateReception.CheckSum;
                else if (msgDecodedPayloadLength > 512)
                    rcvState = StateReception.Waiting;
                else
                {
                    msgDecodedPayload = new byte[msgDecodedPayloadLength];
                    msgDecodedPayloadIndex = 0;
                    rcvState = StateReception.Payload;
                }
                break;

            case StateReception.Payload:
                msgDecodedPayload[msgDecodedPayloadIndex++] = c;
                if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                    rcvState = StateReception.CheckSum;
                break;
            case StateReception.CheckSum:
                var calculatedChecksum = CalculateChecksum(msgDecodedFunction,msgDecodedPayloadLength,msgDecodedPayload);
                var receivedChecksum = c;
                if (calculatedChecksum == receivedChecksum)
                {
                    textBoxReception.Text += "\n Cool";//Success, on a un message valide
                }
                else
                {
                    textBoxReception.Text += "\n Pas Cool";//Success, on a un message valide
                }
                rcvState = StateReception.Waiting;
                break;
            default:
                rcvState = StateReception.Waiting;
                break;
        }
    }

    private void textBoxEmission_TextChanged(object sender, TextChangedEventArgs e)
    {

    }



    byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
    {
        byte checksum = 0;
        checksum ^= 0xFE;
        checksum ^= (byte)(msgFunction >> 8);
        checksum ^= (byte)(msgFunction);
        checksum ^= (byte)(msgPayloadLength >> 8);
        checksum ^= (byte)(msgPayloadLength);
        for (int i = 0; i < msgPayloadLength; i++)
            checksum ^= (byte)(msgPayload[i]);
        return checksum;
    }

    void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
    {
        int taille = 6 + msgPayloadLength;
        byte[] msgSend = new byte[taille];
        int pos = 0;
        msgSend[pos++] = 0xFE;
        msgSend[pos++] = (byte)(msgFunction);
        msgSend[pos++] = (byte)(msgFunction >> 8);
        msgSend[pos++] = (byte)(msgPayloadLength>>8);
        msgSend[pos++] = (byte)(msgPayloadLength );
        for (int i = 0; i < msgPayloadLength; i++)
            msgSend[pos++] = (byte)(msgPayload[i]);
        msgSend[pos++] = (byte)CalculateChecksum(msgFunction, msgPayloadLength,msgPayload);
        serialPort1.Write(msgSend, 0, msgSend.Length);
    }

}
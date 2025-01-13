﻿using System.Text;
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
    string receivedText;
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
        textBoxReception.Text += receivedText;
        receivedText = "";
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
        textBoxReception.Text += ("Reçu :" + "Test" + "\n");
        textBoxEmission.Text = null;
    }

    public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
    {
        receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
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
        Test();
        Clear();
    }


    private void textBoxEmission_TextChanged(object sender, TextChangedEventArgs e)
    {

    }
}
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



namespace RobotInterface;

/// <summary>
/// Interaction logic for MainWindow.xaml
/// </summary>
public partial class MainWindow : Window
{
    
    ExtendedSerialPort serialPort1;
    
    public MainWindow()
    {
        InitializeComponent();
        serialPort1 = new ExtendedSerialPort("COM7", 115200, Parity.None, 8, StopBits.One);
        serialPort1.Open();
    }
    void SendMessage()
    {
        textBoxReception.Text += ("Reçu :" + textBoxEmission.Text + "\n");
        textBoxEmission.Text = null;
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
    
    

    private void textBoxEmission_TextChanged(object sender, TextChangedEventArgs e)
    {

    }
}
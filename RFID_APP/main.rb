class ArduinoService  
    def initialize
        port_str = "COM14"
        baud_rate = 9600
        data_bits = 8
        stop_bits = 1
        parity = SerialPort::NONE 
        
        if port_connected?(port_str)
            sp = SerialPort.new(port_str, baud_rate, data_bits, stop_bits, parity)
            order_confirmation(sp)
        end
    end  
    
    def order_confirmation(sp)
        sp.write('a')
        sp.flush
    end  
    
    def port_connected?(port)
        return true if Dir.glob(port).count == 1
    end
end
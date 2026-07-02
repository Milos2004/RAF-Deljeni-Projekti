clear all
close all
clc

pkg load instrument-control

% Povezivanje sa Arduinom
s = serialport("COM5", 9600);

x = [];
y = [];

figure

while true

    % Čitanje podatka sa serijskog porta
    data = readline(s);

    % Pretvaranje stringa u broj
    temp = str2double(data);

    % Ako je broj validan
    if !isnan(temp)

        x(end+1) = length(x);
        y(end+1) = temp;

        plot(x, y, '-o')

        xlabel('Vreme')
        ylabel('Temperatura [C]')
        title('Temperatura sa LM35 senzora')

        grid on

        drawnow
    endif

endwhile

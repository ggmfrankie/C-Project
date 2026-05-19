library ieee;
use ieee.std_logic_1164.all;

entity Demultiplexer is 
port (
    sel: in std_logic_vector(1 downto 0);
    q: in std_logic;
    a, b, c, d: out std_logic
	
);
end Demultiplexer;

architecture rtl of Demultiplexer is
    
begin
    process(sel, q)
    begin
        a <= '0';
        b <= '0';
        c <= '0';
        d <= '0';

        case sel is
            when "00" => a <= q;
            when "01" => b <= q;
            when "10" => c <= q;
            when others => d <= q;
        end case;   
    end process;
    
end rtl;
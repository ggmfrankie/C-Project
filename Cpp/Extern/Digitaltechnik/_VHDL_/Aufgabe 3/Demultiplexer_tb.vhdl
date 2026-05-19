library ieee;
use ieee.std_logic_1164.all;
entity Demultiplexer_tb is end;
architecture sim of Demultiplexer_tb is
    signal sel : std_logic_vector(1 downto 0) := (others => '0');
    signal q : std_logic := '0';
    signal a : std_logic := '0';
    signal b : std_logic := '0';
    signal c : std_logic := '0';
    signal d : std_logic := '0';
begin
    port_inst: entity work.Demultiplexer
         port map (
            sel => sel,
            q => q,
            a => a,
            b => b,
            c => c,
            d => d
         );

    tb: process
    begin
        sel <= "00";
        q <= '0';
       wait for 250 ns;
        sel <= "00";
        q <= '1';
       wait for 250 ns;
        sel <= "01";
        q <= '0';
       wait for 250 ns;
        sel <= "01";
        q <= '1';
       wait for 250 ns;
        sel <= "10";
        q <= '0';
       wait for 250 ns;
        sel <= "10";
        q <= '1';
       wait for 250 ns;
        sel <= "11";
        q <= '0';
       wait for 250 ns;
        sel <= "11";
        q <= '1';
       wait for 250 ns;
        wait;
    end process;
end architecture;
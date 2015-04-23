--------------------------------------------------------------------
--	   Operateur Norme					  --
--------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
library METAMOR;
use METAMOR.attributes.all;
use METAMOR.array_arith.all;
library SYNOPSYS;
use SYNOPSYS.std_logic_arith.all;
use SYNOPSYS.std_logic_unsigned.all;

entity Norme is
    generic (n : INTEGER :=19);
    port (
        GV, GH : in STD_LOGIC_VECTOR (n-1 downto 0);
        Norme : out STD_LOGIC_VECTOR (n-2 downto 0)
    );
end Norme;


architecture Norme_archX of Norme is

signal Q : STD_LOGIC_VECTOR (2 downto 1);
signal absGV, absGH, I0 : STD_LOGIC_VECTOR (n-1 downto 0);

component Abso is
    generic (n : INTEGER :=19);
    port (
        DATAIN: in STD_LOGIC_VECTOR (n-1 downto 0);
        DATAOUT: out STD_LOGIC_VECTOR (n-1 downto 0)
    );
end component;

begin
	
Av : Abso 
    generic map (N)
    port map(GV, absGV);

Ah : Abso 
    generic map (N)
    port map(GH, absGH);

Q(1) <= '1' when ("0" & absGH) < (absGV & "0") else '0';
Q(2) <= '1' when ("0" & absGV) < (absGH & "0") else '0';

I0 <= ((absGV + absGH) + ("0" & (absGV(N-1 downto 1) + absGH(N-1 downto 1))));

Norme <= absGV(N-1 downto 1) when Q="01" else 
	absGH(N-1 downto 1) when Q="10" else
	I0(N-1 downto 1);

end Norme_archX;

--------------------------------------------------------------------
--   Soustracteur n bits a retenue propagee 			  --
--------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
library METAMOR;
use METAMOR.attributes.all;
use METAMOR.array_arith.all;
library SYNOPSYS;
use SYNOPSYS.std_logic_arith.all;
use SYNOPSYS.std_logic_unsigned.all;

entity Substrac is
    generic (n : INTEGER :=17);
    port (
        a: in STD_LOGIC_VECTOR (n-1 downto 0);
        b: in STD_LOGIC_VECTOR (n-1 downto 0);
        S: out STD_LOGIC_VECTOR (n downto 0)
    );
end Substrac;


architecture Substrac_archX of Substrac is

begin
	S <= ("0" & a) - ("0" & b);
end Substrac_archX;

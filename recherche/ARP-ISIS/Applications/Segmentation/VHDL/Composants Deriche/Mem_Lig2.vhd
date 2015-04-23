---------------------------------------------------------
--  Memoire ligne utilisant la memoire externe
---------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
library SYNOPSYS;
use SYNOPSYS.std_logic_arith.all;
use SYNOPSYS.std_logic_unsigned.all;
library metamor;
use metamor.attributes.all;

entity Mem_Lig2 is
    port (
    FrameIn : in STD_LOGIC_VECTOR (28 downto 0);
    frameOut : out STD_LOGIC_VECTOR (28 downto 0);
    Clk, load, reset : in STD_LOGIC;
    X0Add : out STD_LOGIC_VECTOR (17 downto 0);
    Datain : in STD_LOGIC_VECTOR (28 downto 0);
    Dataout : out STD_LOGIC_VECTOR (28 downto 0);
    X0OE, X0WE : out STD_LOGIC;
    tri : out std_logic);
end Mem_Lig2;

architecture Mem_Lig2_arch of Mem_Lig2 is

type Type_Etat is (S0, S1, S2, S3, S4, S5);

signal pres_state : Type_Etat:=S0;
signal RegOut, RegIn : STD_LOGIC_VECTOR (28 downto 0);
signal A : STD_LOGIC_VECTOR (8 downto 0):="000000000";
signal pair : STD_LOGIC;

  attribute enum_encoding : string;
  attribute enum_encoding of Type_Etat : type is "one hot";  
  attribute critical of pres_state, tri : signal is  true;

begin
  
 process(clk, reset)
 begin
 	if (reset='1') then 
		pres_state <= S0;
		RegOut <= (others=>'0');
		RegIn <= (others=>'0');
		X0OE <= '0';
		X0WE <= '1';
		tri <= '0';
		A <= "000000000";
		pair <= '0';
 	elsif rising_edge(clk) then
		if (pres_state=S0) then
			if (Load='1') then
				pres_state <= S1;
				X0OE <= '0';
				X0WE <= '1';
				tri <= '0';
				RegOut <= Datain;
				RegIn <= FrameIn;
			else
				pres_state <= S0;
				X0OE <= '0';
				X0WE <= '1';
				tri <= '0';
				end if;
		elsif (pres_state=S1) then
				pres_state <= S2;
				X0OE <= '1';
				X0WE <= '1';
				tri <= '0';
				if (pair='0') then
					if (A="111111111") then
						pair <= '1';
					else
						A<=A+1;
					end if;
				else
					if (A="000000000") then
						pair <= '0';
					else
						A<=A-1;
					end if;
				end if;
		elsif (pres_state=S2) then
				pres_state <= S3;
				X0OE <= '1';
				X0WE <= '0';
				tri <= '1';
		elsif (pres_state=S3) then
				pres_state <= S4;
				X0OE <= '1';
				X0WE <= '1';
				tri <= '1';
		elsif (pres_state=S4) then
				pres_state <= S5;
				X0OE <= '1';
				X0WE <= '1';
				tri <= '0';		
		elsif (pres_state=S5) then
				pres_state <= S0;
				X0OE <= '0';
				X0WE <= '1';
				tri <= '0';
		else
				X0OE <= '1';
				X0WE <= '1';
				tri <= '0';
		end if;
 	end if;
 end process;

Dataout<=RegIn;
FrameOut <= RegOut;

X0Add <= "000000000" & A;
 
end Mem_Lig2_arch;

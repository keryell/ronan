-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- --
-- CPM Fifo Output Interface    				--
-- Transmission Core (PipeCore)                                 --
--                                                              --
-- Author:     Serge Savard modifie par N. Viteau               --
-- $State: $                                                    --
-- $Revision: $                                                 --
-- $Date: $                                                     --
--                                                              --
-- $Log: $                                                      --
--                                                              --
-- (c) Copyright MiroTech Microsystems 1995-1996                --
-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- --

library ieee;
use ieee.std_logic_1164.all;

entity fifo_out is
	port (
		--=-- Module Interface --=--
		Engclk		: in std_logic; 
		Frame_in	: in std_logic_vector(7 downto 0);
		FrameAvail	: in std_logic;
		FrameTaken	: out std_logic; 
		RESET_in	: in std_logic; 
		--=-- CPM Interface --=--
		FifoEn		: out std_logic;
		FifoRdy		: in std_logic;
		FifoData	: out std_logic_vector(7 downto 0);
		Load		: out std_logic
	);

end fifo_out;

architecture behavioral of fifo_out is

	-- -=-=-=-=-=-=-=-=-= --
	-- States definitions --
	-- -=-=-=-=-=-=-=-=-= --

	type output_states_def is (PU, WFB1, LB1, ShB1, WFAB1, WFB2, LB2, ShB2, WFAB2, WFB3, LB3, ShB3, WFAB3, WFB4, LB4, WFR, SB2, SB3, SB4 );

	attribute enum_encoding : string;
	attribute enum_encoding of output_states_def	: type is "one hot";

	signal present_out_state, next_out_state : output_states_def := PU;

	-- -=-=-=-=-=-=-=-=-=- --
	-- Signals definitions --
	-- -=-=-=-=-=-=-=-=-=- --

	signal load_sig, shift_sig		: std_logic;

	-- -=-=-=-=-=-=-=-=-=-=-= --
	-- Components definitions --
	-- -=-=-=-=-=-=-=-=-=-=-= --

	component Shifter
	port (
		clk		: in std_logic;
		frame_in	: in std_logic_vector(7 downto 0);
		load_en		: in std_logic;
		shift_en	: in std_logic;
		data_out	: out std_logic_vector(7 downto 0));
	end component;

begin

	-- ----------------------------- --
	-- ++ -- Basic Assignments -- ++ --


	-- ------------------------------------- --
	-- ++ -- State Machine definitions -- ++ --

	FSMdef: process(engclk, RESET_in)
	begin
		if (RESET_in = '1') then
			present_out_state <= PU;
		elsif rising_edge(engclk) then
			present_out_state <= next_out_state;
		end if;
	end process;


	-- ---------------------------------- --
	-- ++ -- State machine behavior -- ++ --

	FSM1: process(present_out_state, FifoRdy, FrameAvail)
	begin
		case present_out_state is
			when PU =>
				next_out_state <= WFB1;
				
			when WFB1 =>
				if (FrameAvail = '1') then
					next_out_state <= LB1;
				else
					next_out_state <= WFB1;
				end if;	
			when LB1 =>
				next_out_state <= ShB1;
			
			when ShB1 =>
				next_out_state <= WFAB1;

			when WFAB1 =>
				if (FrameAvail = '0') then
					next_out_state <= WFB2;
				else
					next_out_state <= WFAB1;
				end if;	

			when WFB2 =>
				if (FrameAvail = '1') then
					next_out_state <= LB2;
				else
					next_out_state <= WFB2;
				end if;	

			when LB2 =>
				next_out_state <= ShB2;
			
			when ShB2 =>
				next_out_state <= WFAB2;

			when WFAB2 =>
				if (FrameAvail = '0') then
					next_out_state <= WFB3;
				else
					next_out_state <= WFAB2;
				end if;

			when WFB3 =>
				if (FrameAvail = '1') then
					next_out_state <= LB3;
				else
					next_out_state <= WFB3;
				end if;	

			when LB3 =>
				next_out_state <= ShB3;
			
			when ShB3 =>
				next_out_state <= WFAB3;

			when WFAB3 =>
				if (FrameAvail = '0') then
					next_out_state <= WFB4;
				else
					next_out_state <= WFAB3;
				end if;

			when WFB4 =>
				if (FrameAvail = '1') then
					next_out_state <= LB4;
				else
					next_out_state <= WFB4;
				end if;	

			when LB4 =>
				next_out_state <= WFR;

			when WFR =>
				if (FifoRdy = '1') then
					next_out_state <= SB2;
				else
					next_out_state <= WFR;
				end if;

			when SB2 =>
				next_out_state <= SB3;

			when SB3 =>
				next_out_state <= SB4;

			when SB4 =>
				next_out_state <= WFB1;

		end case;
	end process;


	-- ------------------------------------------- --
	-- ++ -- State Machine Dependant Signals -- ++ --

	FifoEn <= '0' when ((present_out_state = WFR) or
                        (present_out_state = SB2) or
                        (present_out_state = SB3) or
                        (present_out_state = SB4)) else '1';

	FrameTaken <= '1' when ((present_out_state = WFAB1) or
                        (present_out_state = WFAB2) or
                        (present_out_state = WFAB3) or
                        (present_out_state = WFR)) else '0';

	load_sig <= '1' when ((present_out_state = LB1) or
                        (present_out_state = LB2) or
                        (present_out_state = LB3) or
                        (present_out_state = LB4)) else '0';

	shift_sig <= '1' when ((present_out_state = ShB1) or
                        (present_out_state = ShB2) or
                        (present_out_state = ShB3) or
                        (present_out_state = WFR and FifoRdy = '1') or
			(present_out_state = SB2) or
			(present_out_state = SB3) or
			(present_out_state = SB4) ) else '0';

	Load <= load_sig;

	-- ---------------------------- --
	-- ++ -- ReFraming Buffer -- ++ --

	Shft : Shifter port map
		(
			clk => engclk,
			frame_in => Frame_in,
			load_en => load_sig,
			shift_en => shift_sig,
			data_out => fifoData);


end behavioral;


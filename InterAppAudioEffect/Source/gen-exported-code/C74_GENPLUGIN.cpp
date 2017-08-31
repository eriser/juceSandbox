#include "C74_GENPLUGIN.h"

namespace C74_GENPLUGIN {

/*******************************************************************************************************************
Cycling '74 License for Max-Generated Code for Export
Copyright (c) 2016 Cycling '74
The code that Max generates automatically and that end users are capable of exporting and using, and any
  associated documentation files (the “Software”) is a work of authorship for which Cycling '74 is the author
  and owner for copyright purposes.  A license is hereby granted, free of charge, to any person obtaining a
  copy of the Software (“Licensee”) to use, copy, modify, merge, publish, and distribute copies of the Software,
  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The Software is licensed to Licensee only for non-commercial use. Users who wish to make commercial use of the
  Software must contact the copyright owner to determine if a license for commercial use is available, and the
  terms and conditions for same, which may include fees or royalties. For commercial use, please send inquiries
  to licensing (at) cycling74.com.  The determination of whether a use is commercial use or non-commercial use is based
  upon the use, not the user. The Software may be used by individuals, institutions, governments, corporations, or
  other business whether for-profit or non-profit so long as the use itself is not a commercialization of the
  materials or a use that generates or is intended to generate income, revenue, sales or profit.
The above copyright notice and this license shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*******************************************************************************************************************/

// global noise generator
Noise noise;
static const int GENLIB_LOOPCOUNT_BAIL = 100000;


// The State struct contains all the state and procedures for the gendsp kernel
typedef struct State {
	CommonState __commonstate;
	Delay m_delay_44;
	Delay m_delay_24;
	Delay m_delay_25;
	Delay m_delay_27;
	Delay m_delay_23;
	Delay m_delay_43;
	Delay m_delay_22;
	Delay m_delay_19;
	Delay m_delay_17;
	Delay m_delay_29;
	Delay m_delay_33;
	Delay m_delay_41;
	Delay m_delay_42;
	Delay m_delay_31;
	Delay m_delay_39;
	Delay m_delay_35;
	Delay m_delay_37;
	Delay m_delay_15;
	Delay m_delay_21;
	Delay m_delay_9;
	Delay m_delay_5;
	Delay m_delay_13;
	Delay m_delay_7;
	Delay m_delay_11;
	int __exception;
	int vectorsize;
	t_sample m_history_36;
	t_sample m_spread_3;
	t_sample m_history_34;
	t_sample m_history_40;
	t_sample m_fb_1;
	t_sample m_history_38;
	t_sample m_fb_2;
	t_sample samplerate;
	t_sample m_damp_4;
	t_sample m_history_30;
	t_sample m_history_16;
	t_sample m_history_12;
	t_sample m_history_20;
	t_sample m_history_18;
	t_sample m_history_32;
	t_sample m_history_10;
	t_sample m_history_26;
	t_sample m_history_8;
	t_sample m_history_6;
	t_sample m_history_28;
	t_sample m_history_14;
	// re-initialize all member variables;
	inline void reset(t_param __sr, int __vs) {
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_fb_1 = ((t_sample)0.5);
		m_fb_2 = ((t_sample)0.9);
		m_spread_3 = ((int)0);
		m_damp_4 = ((t_sample)0.5);
		m_delay_5.reset("m_delay_5", ((int)2000));
		m_history_6 = ((int)0);
		m_delay_7.reset("m_delay_7", ((int)2000));
		m_history_8 = ((int)0);
		m_delay_9.reset("m_delay_9", ((int)2000));
		m_history_10 = ((int)0);
		m_delay_11.reset("m_delay_11", ((int)2000));
		m_history_12 = ((int)0);
		m_delay_13.reset("m_delay_13", ((int)2000));
		m_history_14 = ((int)0);
		m_delay_15.reset("m_delay_15", ((int)2000));
		m_history_16 = ((int)0);
		m_delay_17.reset("m_delay_17", ((int)2000));
		m_history_18 = ((int)0);
		m_delay_19.reset("m_delay_19", ((int)2000));
		m_history_20 = ((int)0);
		m_delay_21.reset("m_delay_21", ((int)2000));
		m_delay_22.reset("m_delay_22", ((int)2000));
		m_delay_23.reset("m_delay_23", ((int)2000));
		m_delay_24.reset("m_delay_24", ((int)2000));
		m_delay_25.reset("m_delay_25", ((int)2000));
		m_history_26 = ((int)0);
		m_delay_27.reset("m_delay_27", ((int)2000));
		m_history_28 = ((int)0);
		m_delay_29.reset("m_delay_29", ((int)2000));
		m_history_30 = ((int)0);
		m_delay_31.reset("m_delay_31", ((int)2000));
		m_history_32 = ((int)0);
		m_delay_33.reset("m_delay_33", ((int)2000));
		m_history_34 = ((int)0);
		m_delay_35.reset("m_delay_35", ((int)2000));
		m_history_36 = ((int)0);
		m_delay_37.reset("m_delay_37", ((int)2000));
		m_history_38 = ((int)0);
		m_delay_39.reset("m_delay_39", ((int)2000));
		m_history_40 = ((int)0);
		m_delay_41.reset("m_delay_41", ((int)2000));
		m_delay_42.reset("m_delay_42", ((int)2000));
		m_delay_43.reset("m_delay_43", ((int)2000));
		m_delay_44.reset("m_delay_44", ((int)2000));
		genlib_reset_complete(this);
		
	};
	// the signal processing routine;
	inline int perform(t_sample ** __ins, t_sample ** __outs, int __n) {
		vectorsize = __n;
		const t_sample * __in1 = __ins[0];
		const t_sample * __in2 = __ins[1];
		t_sample * __out1 = __outs[0];
		t_sample * __out2 = __outs[1];
		if (__exception) {
			return __exception;
			
		} else if (( (__in1 == 0) || (__in2 == 0) || (__out1 == 0) || (__out2 == 0) )) {
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		t_sample mul_606 = (m_fb_1 * ((t_sample)0.5));
		t_sample mul_663 = (m_fb_1 * ((t_sample)0.5));
		t_sample add_592 = (((int)225) + m_spread_3);
		t_sample add_649 = (((int)225) + m_spread_3);
		t_sample add_594 = (((int)341) + m_spread_3);
		t_sample add_651 = (((int)341) + m_spread_3);
		t_sample add_604 = (((int)441) + m_spread_3);
		t_sample add_661 = (((int)441) + m_spread_3);
		t_sample add_590 = (((int)556) + m_spread_3);
		t_sample add_647 = (((int)556) + m_spread_3);
		t_sample damp_560 = m_damp_4;
		t_sample damp_559 = damp_560;
		t_sample damp_566 = damp_560;
		t_sample damp_565 = damp_560;
		t_sample damp_564 = damp_560;
		t_sample damp_563 = damp_560;
		t_sample damp_562 = damp_560;
		t_sample damp_561 = damp_560;
		t_sample damp_632 = damp_560;
		t_sample damp_631 = damp_560;
		t_sample damp_633 = damp_560;
		t_sample damp_634 = damp_560;
		t_sample damp_635 = damp_560;
		t_sample damp_636 = damp_560;
		t_sample damp_637 = damp_560;
		t_sample damp_638 = damp_560;
		t_sample add_596 = (((int)1617) + m_spread_3);
		t_sample rsub_567 = (((int)1) - damp_559);
		t_sample add_603 = (((int)1116) + m_spread_3);
		t_sample rsub_670 = (((int)1) - damp_566);
		t_sample add_602 = (((int)1188) + m_spread_3);
		t_sample rsub_682 = (((int)1) - damp_565);
		t_sample add_601 = (((int)1277) + m_spread_3);
		t_sample rsub_698 = (((int)1) - damp_564);
		t_sample add_600 = (((int)1356) + m_spread_3);
		t_sample rsub_710 = (((int)1) - damp_563);
		t_sample add_599 = (((int)1422) + m_spread_3);
		t_sample rsub_721 = (((int)1) - damp_562);
		t_sample add_598 = (((int)1491) + m_spread_3);
		t_sample rsub_731 = (((int)1) - damp_561);
		t_sample add_597 = (((int)1557) + m_spread_3);
		t_sample rsub_743 = (((int)1) - damp_560);
		t_sample add_654 = (((int)1557) + m_spread_3);
		t_sample rsub_777 = (((int)1) - damp_632);
		t_sample add_653 = (((int)1617) + m_spread_3);
		t_sample rsub_789 = (((int)1) - damp_631);
		t_sample add_655 = (((int)1491) + m_spread_3);
		t_sample rsub_801 = (((int)1) - damp_633);
		t_sample add_656 = (((int)1422) + m_spread_3);
		t_sample rsub_809 = (((int)1) - damp_634);
		t_sample add_657 = (((int)1356) + m_spread_3);
		t_sample rsub_821 = (((int)1) - damp_635);
		t_sample add_658 = (((int)1277) + m_spread_3);
		t_sample rsub_833 = (((int)1) - damp_636);
		t_sample add_659 = (((int)1188) + m_spread_3);
		t_sample rsub_843 = (((int)1) - damp_637);
		t_sample add_660 = (((int)1116) + m_spread_3);
		t_sample rsub_855 = (((int)1) - damp_638);
		// the main sample loop;
		while ((__n--)) {
			const t_sample in1 = (*(__in1++));
			const t_sample in2 = (*(__in2++));
			t_sample mul_608 = (in2 * ((t_sample)0.015));
			t_sample tap_574 = m_delay_5.read_linear(add_596);
			t_sample gen_607 = tap_574;
			t_sample mul_572 = (tap_574 * damp_559);
			t_sample mul_570 = (m_history_6 * rsub_567);
			t_sample add_571 = (mul_572 + mul_570);
			t_sample mul_568 = (add_571 * m_fb_2);
			t_sample add_575 = (mul_608 + mul_568);
			t_sample history_569_next_576 = fixdenorm(add_571);
			t_sample tap_669 = m_delay_7.read_linear(add_603);
			t_sample gen_577 = tap_669;
			t_sample mul_666 = (tap_669 * damp_566);
			t_sample mul_667 = (m_history_8 * rsub_670);
			t_sample add_668 = (mul_666 + mul_667);
			t_sample mul_675 = (add_668 * m_fb_2);
			t_sample add_673 = (mul_608 + mul_675);
			t_sample history_569_next_672 = fixdenorm(add_668);
			t_sample tap_681 = m_delay_9.read_linear(add_602);
			t_sample gen_578 = tap_681;
			t_sample mul_678 = (tap_681 * damp_565);
			t_sample mul_679 = (m_history_10 * rsub_682);
			t_sample add_680 = (mul_678 + mul_679);
			t_sample mul_687 = (add_680 * m_fb_2);
			t_sample add_686 = (mul_608 + mul_687);
			t_sample history_569_next_685 = fixdenorm(add_680);
			t_sample tap_694 = m_delay_11.read_linear(add_601);
			t_sample gen_579 = tap_694;
			t_sample mul_690 = (tap_694 * damp_564);
			t_sample mul_692 = (m_history_12 * rsub_698);
			t_sample add_693 = (mul_690 + mul_692);
			t_sample mul_699 = (add_693 * m_fb_2);
			t_sample add_696 = (mul_608 + mul_699);
			t_sample history_569_next_695 = fixdenorm(add_693);
			t_sample tap_706 = m_delay_13.read_linear(add_600);
			t_sample gen_580 = tap_706;
			t_sample mul_702 = (tap_706 * damp_563);
			t_sample mul_704 = (m_history_14 * rsub_710);
			t_sample add_705 = (mul_702 + mul_704);
			t_sample mul_711 = (add_705 * m_fb_2);
			t_sample add_708 = (mul_608 + mul_711);
			t_sample history_569_next_707 = fixdenorm(add_705);
			t_sample tap_717 = m_delay_15.read_linear(add_599);
			t_sample gen_581 = tap_717;
			t_sample mul_714 = (tap_717 * damp_562);
			t_sample mul_715 = (m_history_16 * rsub_721);
			t_sample add_716 = (mul_714 + mul_715);
			t_sample mul_723 = (add_716 * m_fb_2);
			t_sample add_719 = (mul_608 + mul_723);
			t_sample history_569_next_718 = fixdenorm(add_716);
			t_sample tap_730 = m_delay_17.read_linear(add_598);
			t_sample gen_582 = tap_730;
			t_sample mul_726 = (tap_730 * damp_561);
			t_sample mul_728 = (m_history_18 * rsub_731);
			t_sample add_729 = (mul_726 + mul_728);
			t_sample mul_733 = (add_729 * m_fb_2);
			t_sample add_735 = (mul_608 + mul_733);
			t_sample history_569_next_734 = fixdenorm(add_729);
			t_sample tap_742 = m_delay_19.read_linear(add_597);
			t_sample gen_583 = tap_742;
			t_sample mul_738 = (tap_742 * damp_560);
			t_sample mul_740 = (m_history_20 * rsub_743);
			t_sample add_741 = (mul_738 + mul_740);
			t_sample mul_746 = (add_741 * m_fb_2);
			t_sample add_745 = (mul_608 + mul_746);
			t_sample history_569_next_747 = fixdenorm(add_741);
			t_sample add_605 = ((((((((gen_583 + gen_582) + gen_581) + gen_580) + gen_579) + gen_578) + gen_577) + gen_607) + ((int)0));
			t_sample tap_588 = m_delay_21.read_linear(add_590);
			t_sample sub_584 = (add_605 - tap_588);
			t_sample mul_586 = (tap_588 * mul_606);
			t_sample add_585 = (add_605 + mul_586);
			t_sample tap_754 = m_delay_22.read_linear(add_604);
			t_sample sub_750 = (sub_584 - tap_754);
			t_sample mul_753 = (tap_754 * mul_606);
			t_sample add_751 = (sub_584 + mul_753);
			t_sample tap_760 = m_delay_23.read_linear(add_594);
			t_sample sub_756 = (sub_750 - tap_760);
			t_sample mul_759 = (tap_760 * mul_606);
			t_sample add_757 = (sub_750 + mul_759);
			t_sample tap_766 = m_delay_24.read_linear(add_592);
			t_sample sub_762 = (sub_756 - tap_766);
			t_sample mul_765 = (tap_766 * mul_606);
			t_sample add_763 = (sub_756 + mul_765);
			t_sample out2 = sub_762;
			t_sample mul_665 = (in1 * ((t_sample)0.015));
			t_sample tap_772 = m_delay_25.read_linear(add_654);
			t_sample gen_645 = tap_772;
			t_sample mul_768 = (tap_772 * damp_632);
			t_sample mul_770 = (m_history_26 * rsub_777);
			t_sample add_771 = (mul_768 + mul_770);
			t_sample mul_775 = (add_771 * m_fb_2);
			t_sample add_773 = (mul_665 + mul_775);
			t_sample history_569_next_776 = fixdenorm(add_771);
			t_sample tap_784 = m_delay_27.read_linear(add_653);
			t_sample gen_664 = tap_784;
			t_sample mul_780 = (tap_784 * damp_631);
			t_sample mul_782 = (m_history_28 * rsub_789);
			t_sample add_783 = (mul_780 + mul_782);
			t_sample mul_787 = (add_783 * m_fb_2);
			t_sample add_785 = (mul_665 + mul_787);
			t_sample history_569_next_788 = fixdenorm(add_783);
			t_sample tap_796 = m_delay_29.read_linear(add_655);
			t_sample gen_644 = tap_796;
			t_sample mul_792 = (tap_796 * damp_633);
			t_sample mul_794 = (m_history_30 * rsub_801);
			t_sample add_795 = (mul_792 + mul_794);
			t_sample mul_799 = (add_795 * m_fb_2);
			t_sample add_797 = (mul_665 + mul_799);
			t_sample history_569_next_800 = fixdenorm(add_795);
			t_sample tap_808 = m_delay_31.read_linear(add_656);
			t_sample gen_643 = tap_808;
			t_sample mul_804 = (tap_808 * damp_634);
			t_sample mul_806 = (m_history_32 * rsub_809);
			t_sample add_807 = (mul_804 + mul_806);
			t_sample mul_812 = (add_807 * m_fb_2);
			t_sample add_810 = (mul_665 + mul_812);
			t_sample history_569_next_813 = fixdenorm(add_807);
			t_sample tap_820 = m_delay_33.read_linear(add_657);
			t_sample gen_642 = tap_820;
			t_sample mul_816 = (tap_820 * damp_635);
			t_sample mul_818 = (m_history_34 * rsub_821);
			t_sample add_819 = (mul_816 + mul_818);
			t_sample mul_825 = (add_819 * m_fb_2);
			t_sample add_822 = (mul_665 + mul_825);
			t_sample history_569_next_823 = fixdenorm(add_819);
			t_sample tap_832 = m_delay_35.read_linear(add_658);
			t_sample gen_641 = tap_832;
			t_sample mul_828 = (tap_832 * damp_636);
			t_sample mul_830 = (m_history_36 * rsub_833);
			t_sample add_831 = (mul_828 + mul_830);
			t_sample mul_836 = (add_831 * m_fb_2);
			t_sample add_835 = (mul_665 + mul_836);
			t_sample history_569_next_837 = fixdenorm(add_831);
			t_sample tap_847 = m_delay_37.read_linear(add_659);
			t_sample gen_640 = tap_847;
			t_sample mul_840 = (tap_847 * damp_637);
			t_sample mul_848 = (m_history_38 * rsub_843);
			t_sample add_842 = (mul_840 + mul_848);
			t_sample mul_845 = (add_842 * m_fb_2);
			t_sample add_844 = (mul_665 + mul_845);
			t_sample history_569_next_846 = fixdenorm(add_842);
			t_sample tap_859 = m_delay_39.read_linear(add_660);
			t_sample gen_639 = tap_859;
			t_sample mul_852 = (tap_859 * damp_638);
			t_sample mul_860 = (m_history_40 * rsub_855);
			t_sample add_854 = (mul_852 + mul_860);
			t_sample mul_857 = (add_854 * m_fb_2);
			t_sample add_856 = (mul_665 + mul_857);
			t_sample history_569_next_858 = fixdenorm(add_854);
			t_sample add_662 = ((((((((gen_639 + gen_640) + gen_641) + gen_642) + gen_643) + gen_644) + gen_664) + gen_645) + ((int)0));
			t_sample tap_868 = m_delay_41.read_linear(add_647);
			t_sample sub_864 = (add_662 - tap_868);
			t_sample mul_867 = (tap_868 * mul_663);
			t_sample add_865 = (add_662 + mul_867);
			t_sample tap_874 = m_delay_42.read_linear(add_661);
			t_sample sub_870 = (sub_864 - tap_874);
			t_sample mul_872 = (tap_874 * mul_663);
			t_sample add_871 = (sub_864 + mul_872);
			t_sample tap_878 = m_delay_43.read_linear(add_651);
			t_sample sub_876 = (sub_870 - tap_878);
			t_sample mul_880 = (tap_878 * mul_663);
			t_sample add_877 = (sub_870 + mul_880);
			t_sample tap_884 = m_delay_44.read_linear(add_649);
			t_sample sub_882 = (sub_876 - tap_884);
			t_sample mul_886 = (tap_884 * mul_663);
			t_sample add_883 = (sub_876 + mul_886);
			t_sample out1 = sub_882;
			m_delay_5.write(add_575);
			m_delay_44.write(add_883);
			m_delay_43.write(add_877);
			m_delay_42.write(add_871);
			m_delay_41.write(add_865);
			m_history_40 = history_569_next_858;
			m_delay_39.write(add_856);
			m_history_38 = history_569_next_846;
			m_delay_37.write(add_844);
			m_history_36 = history_569_next_837;
			m_delay_35.write(add_835);
			m_history_34 = history_569_next_823;
			m_delay_33.write(add_822);
			m_history_32 = history_569_next_813;
			m_delay_31.write(add_810);
			m_history_30 = history_569_next_800;
			m_delay_29.write(add_797);
			m_history_28 = history_569_next_788;
			m_delay_27.write(add_785);
			m_history_26 = history_569_next_776;
			m_delay_25.write(add_773);
			m_delay_24.write(add_763);
			m_delay_23.write(add_757);
			m_delay_22.write(add_751);
			m_delay_21.write(add_585);
			m_history_20 = history_569_next_747;
			m_delay_19.write(add_745);
			m_history_18 = history_569_next_734;
			m_delay_17.write(add_735);
			m_history_16 = history_569_next_718;
			m_delay_15.write(add_719);
			m_history_14 = history_569_next_707;
			m_delay_13.write(add_708);
			m_history_12 = history_569_next_695;
			m_delay_11.write(add_696);
			m_history_10 = history_569_next_685;
			m_delay_9.write(add_686);
			m_history_8 = history_569_next_672;
			m_delay_7.write(add_673);
			m_history_6 = history_569_next_576;
			m_delay_5.step();
			m_delay_7.step();
			m_delay_9.step();
			m_delay_11.step();
			m_delay_13.step();
			m_delay_15.step();
			m_delay_17.step();
			m_delay_19.step();
			m_delay_21.step();
			m_delay_22.step();
			m_delay_23.step();
			m_delay_24.step();
			m_delay_25.step();
			m_delay_27.step();
			m_delay_29.step();
			m_delay_31.step();
			m_delay_33.step();
			m_delay_35.step();
			m_delay_37.step();
			m_delay_39.step();
			m_delay_41.step();
			m_delay_42.step();
			m_delay_43.step();
			m_delay_44.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_fb2(t_param _value) {
		m_fb_1 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_fb1(t_param _value) {
		m_fb_2 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_spread(t_param _value) {
		m_spread_3 = (_value < 0 ? 0 : (_value > 400 ? 400 : _value));
	};
	inline void set_damp(t_param _value) {
		m_damp_4 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	
} State;


///
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs

int gen_kernel_numins = 2;
int gen_kernel_numouts = 2;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 4; }

/// Assistive lables for the signal inputs and outputs

const char *gen_kernel_innames[] = { "in1", "in2" };
const char *gen_kernel_outnames[] = { "out1", "out2" };

/// Invoke the signal process of a State object

int perform(CommonState *cself, t_sample **ins, long numins, t_sample **outs, long numouts, long n) {
	State* self = (State *)cself;
	return self->perform(ins, outs, n);
}

/// Reset all parameters and stateful operators of a State object

void reset(CommonState *cself) {
	State* self = (State *)cself;
	self->reset(cself->sr, cself->vs);
}

/// Set a parameter of a State object

void setparameter(CommonState *cself, long index, t_param value, void *ref) {
	State *self = (State *)cself;
	switch (index) {
		case 0: self->set_damp(value); break;
		case 1: self->set_fb1(value); break;
		case 2: self->set_fb2(value); break;
		case 3: self->set_spread(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_damp_4; break;
		case 1: *value = self->m_fb_2; break;
		case 2: *value = self->m_fb_1; break;
		case 3: *value = self->m_spread_3; break;
		
		default: break;
	}
}

/// Get the name of a parameter of a State object

const char *getparametername(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].name;
	}
	return 0;
}

/// Get the minimum value of a parameter of a State object

t_param getparametermin(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmin;
	}
	return 0;
}

/// Get the maximum value of a parameter of a State object

t_param getparametermax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmax;
	}
	return 0;
}

/// Get parameter of a State object has a minimum and maximum value

char getparameterhasminmax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].hasminmax;
	}
	return 0;
}

/// Get the units of a parameter of a State object

const char *getparameterunits(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].units;
	}
	return 0;
}

/// Get the size of the state of all parameters of a State object

size_t getstatesize(CommonState *cself) {
	return genlib_getstatesize(cself, &getparameter);
}

/// Get the state of all parameters of a State object

short getstate(CommonState *cself, char *state) {
	return genlib_getstate(cself, state, &getparameter);
}

/// set the state of all parameters of a State object

short setstate(CommonState *cself, const char *state) {
	return genlib_setstate(cself, state, &setparameter);
}

/// Allocate and configure a new State object and it's internal CommonState:

void *create(t_param sr, long vs) {
	State *self = new State;
	self->reset(sr, vs);
	ParamInfo *pi;
	self->__commonstate.inputnames = gen_kernel_innames;
	self->__commonstate.outputnames = gen_kernel_outnames;
	self->__commonstate.numins = gen_kernel_numins;
	self->__commonstate.numouts = gen_kernel_numouts;
	self->__commonstate.sr = sr;
	self->__commonstate.vs = vs;
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(4 * sizeof(ParamInfo));
	self->__commonstate.numparams = 4;
	// initialize parameter 0 ("m_damp_4")
	pi = self->__commonstate.params + 0;
	pi->name = "damp";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_damp_4;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_fb_2")
	pi = self->__commonstate.params + 1;
	pi->name = "fb1";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_fb_2;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_fb_1")
	pi = self->__commonstate.params + 2;
	pi->name = "fb2";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_fb_1;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_spread_3")
	pi = self->__commonstate.params + 3;
	pi->name = "spread";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_spread_3;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 400;
	pi->exp = 0;
	pi->units = "";		// no units defined
	
	return self;
}

/// Release all resources and memory used by a State object:

void destroy(CommonState *cself) {
	State *self = (State *)cself;
	genlib_sysmem_freeptr(cself->params);
		
	delete self;
}


} // C74_GENPLUGIN::

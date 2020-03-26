#include "UtilityFunc.h"

std::string Utility::toString(const sf::Keyboard::Key & val)
{
	using namespace sf;

	using K = Keyboard;

	switch (val)
	{
	case K::A:
		return std::string("A");
		break;

	case K::B:
		return std::string("B");
		break;

	case K::C:
		return std::string("C");
		break;

	case K::D:
		return std::string("D");
		break;

	case K::E:
		return std::string("E");
		break;

	case K::F:
		return std::string("F");
		break;

	case K::G:
		return std::string("G");
		break;

	case K::H:
		return std::string("H");
		break;

	case K::I:
		return std::string("I");
		break;

	case K::J:
		return std::string("J");
		break;

	case K::K:
		return std::string("K");
		break;

	case K::L:
		return std::string("L");
		break;

	case K::M:
		return std::string("M");
		break;

	case K::N:
		return std::string("N");
		break;

	case K::O:
		return std::string("O");
		break;

	case K::P:
		return std::string("P");
		break;

	case K::Q:
		return std::string("Q");
		break;

	case K::R:
		return std::string("R");
		break;

	case K::S:
		return std::string("S");
		break;

	case K::T:
		return std::string("T");
		break;

	case K::U:
		return std::string("U");
		break;

	case K::V:
		return std::string("V");
		break;

	case K::W:
		return std::string("W");
		break;

	case K::X:
		return std::string("X");
		break;

	case K::Y:
		return std::string("Y");
		break;

	case K::Z:
		return std::string("Z");
		break;
	case K::Add:
		return std::string("+");
		break;
	case K::BackSlash:
		return std::string("\\");
		break;
	case K::BackSpace:
		return std::string("BackSpace");
		break;
	case K::Comma:
		return std::string(",");
		break;
	case K::Dash:
		return std::string("-");
		break;
	case K::Delete:
		return std::string("Delete");
		break;
	case K::Divide:
		return std::string("Divide");
		break;
	case K::Down:
		return std::string("Down");
		break;
	case K::End:
		return std::string("End");
		break;
	case K::Enter:
		return std::string("Enter");
		break;
	case K::Equal:
		return std::string("=");
		break;
	case K::Escape:
		return std::string("Escape");
		break;
	case K::F1:
		return std::string("F1");
		break;
	case K::F10:
		return std::string("F10");
		break;
	case K::F11:
		return std::string("F11");
		break;
	case K::F12:
		return std::string("F12");
		break;
	case K::F13:
		return std::string("F13");
		break;
	case K::F14:
		return std::string("F14");
		break;
	case K::F15:
		return std::string("F15");
		break;
	case K::F2:
		return std::string("F2");
		break;
	case K::F3:
		return std::string("F3");
		break;
	case K::F4:
		return std::string("F4");
		break;
	case K::F5:
		return std::string("F5");
		break;
	case K::F6:
		return std::string("F6");
		break;
	case K::F7:
		return std::string("F7");
		break;
	case K::F8:
		return std::string("F8");
		break;
	case K::F9:
		return std::string("F9");
		break;
	case K::Home:
		return std::string("Home");
		break;
	case K::Insert:
		return std::string("Insert");
		break;
	case K::LAlt:
		return std::string("LAlt");
		break;
	case K::LBracket:
		return std::string("LBracket");
		break;
	case K::LControl:
		return std::string("LControl");
		break;
	case K::Left:
		return std::string("Left");
		break;
	case K::LShift:
		return std::string("LShift");
		break;
	case K::LSystem:
		return std::string("LSystem");
		break;
	case K::Multiply:
		return std::string("Multiply");
		break;
	case K::RAlt:
		return std::string("RAlt");
		break;
	case K::RBracket:
		return std::string("RBracket");
		break;
	case K::RControl:
		return std::string("RControl");
		break;
	case K::Right:
		return std::string("Right");
		break;
	case K::RShift:
		return std::string("RShift");
		break;
	case K::RSystem:
		return std::string("RSystem");
		break;
	case K::Num0:
		return std::string("Num0");
		break;
	case K::Num1:
		return std::string("Num1");
		break;
	case K::Num2:
		return std::string("Num2");
		break;
	case K::Num3:
		return std::string("Num3");
		break;
	case K::Num4:
		return std::string("Num4");
		break;
	case K::Num5:
		return std::string("Num5");
		break;
	case K::Num6:
		return std::string("Num6");
		break;
	case K::Num7:
		return std::string("Num7");
		break;
	case K::Num8:
		return std::string("Num8");
		break;
	case K::Num9:
		return std::string("Num9");
		break;
	case K::Menu:
		return std::string("Menu");
		break;
	case K::Numpad0:
		return std::string("Numpad0");
		break;
	case K::Numpad1:
		return std::string("Numpad1");
		break;
	case K::Numpad2:
		return std::string("Numpad2");
		break;
	case K::Numpad3:
		return std::string("Numpad3");
		break;
	case K::Numpad4:
		return std::string("Numpad4");
		break;
	case K::Numpad5:
		return std::string("Numpad5");
		break;
	case K::Numpad6:
		return std::string("Numpad6");
		break;
	case K::Numpad7:
		return std::string("Numpad7");
		break;
	case K::Numpad8:
		return std::string("Numpad8");
		break;
	case K::Numpad9:
		return std::string("Numpad9");
		break;
	case K::PageDown:
		return std::string("PageDown");
		break;
	case K::PageUp:
		return std::string("PageUp");
		break;
	case K::Pause:
		return std::string("Pause");
		break;
	case K::Period:
		return std::string(".");
		break;
	case K::Quote:
		return std::string("\"");
		break;
	case K::SemiColon:
		return std::string(";");
		break;
	case K::Slash:
		return std::string("Slash");
		break;
	case K::Space:
		return std::string("Space");
		break;
	case K::Subtract:
		return std::string("Subtract");
		break;
	case K::Tab:
		return std::string("Tab");
		break;
	case K::Tilde:
		return std::string("Tilde");
		break;
	case K::Up:
		return std::string("Up");
		break;
	default:
		return std::string("Unknown");
		break;
	}
}

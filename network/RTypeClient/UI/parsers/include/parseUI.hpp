#ifndef PARSEUI
    #define PARSEUI
    #include "../../include/librairies.hpp"
    #include "../../include/UI.hpp"
    #include "./parseButtonElement.hpp"
    #include "./parseCircleElement.hpp"
    #include "./parseInputFieldElement.hpp"
    #include "./parseRectangleElement.hpp"
    #include "./parseTextureElement.hpp"
    #include "./parseTextElement.hpp"
    #include "./parseChoiceFieldElement.hpp"

std::shared_ptr<UI> parseUI(const libconfig::Setting& uiSetting)
{
    try {
        std::string uiId;
        uiSetting.lookupValue("id", uiId);

        auto ui = std::make_shared<UI>(uiId);

        if (uiSetting.exists("elements")) {
            const libconfig::Setting& elements = uiSetting.lookup("elements");
            for (int i = 0; i < elements.getLength(); ++i) {
                const libconfig::Setting& el = elements[i];
                std::string type;
                el.lookupValue("type", type);

                std::shared_ptr<UIElement> element = nullptr;

                if (type == "text")
                    element = parseTextElement(el);
                else if (type == "rectangle")
                    element = parseRectangleElement(el);
                else if (type == "circle")
                    element = parseCircleElement(el);
                else if (type == "texture")
                    element = parseTextureElement(el);
                else if (type == "inputfield")
                    element = parseInputFieldElement(el);
                else if (type == "button")
                    element = parseButtonElement(el);
                else if (type == "choicefield")
                    element = parseChoiceFieldElement(el);

                if (element)
                    ui->addElement(element);
            }
        }

        return ui;
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "parseUI: champ manquant (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "parseUI: type invalide (" << e.getPath() << ")\n";
    }
    return nullptr;
}
#endif
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class IndentationStrategy
{
public:
    virtual ~IndentationStrategy() = default;

    virtual void LineBreak(std::ostream& os) const = 0;
    virtual void ExtraSpace(std::ostream& os) const = 0;
    virtual void MakeIndent(std::ostream& os, const size_t height) const = 0;
};

class NoIndentStrategy : public IndentationStrategy
{
    void LineBreak(std::ostream& os) const override {}
    void ExtraSpace(std::ostream& os) const override { os << ' '; };
    void MakeIndent(std::ostream& os, const size_t height) const override {}
};

class IndentStrategy : public IndentationStrategy
{
public:
    explicit IndentStrategy(const int indent) : indent{ indent } {}

    void LineBreak(std::ostream& os) const override { os << std::endl; }
    void ExtraSpace(std::ostream& os) const override {}
    void MakeIndent(std::ostream& os, const size_t height) const override { os << std::string(height * indent, ' '); }

private:
    const int indent;
};

class ValueComponent
{
public:
    virtual ~ValueComponent() = default;

    virtual void Write(std::ostream& os) const = 0;
    
    virtual void IncreaseHeight() {}
    virtual void SetIndentationStrategy(std::shared_ptr<IndentationStrategy> strategy) { indentationStrategy = strategy; }

protected:
    std::shared_ptr<IndentationStrategy> indentationStrategy;
    int height{ 0 };
};

class Object : public ValueComponent
{
private:
    using KeyValuePair = std::pair<std::string, std::unique_ptr<ValueComponent>>;

public:
    void Add(const std::string& key, std::unique_ptr<ValueComponent> value) {
        value->IncreaseHeight();
        value->SetIndentationStrategy(indentationStrategy);
        items.emplace_back(key, std::move(value));
    }

    void Write(std::ostream& os) const override {
        if (!indentationStrategy) throw std::runtime_error{ "Indentation strategy is not set!" };

        os << '{';
        if (!Empty()) indentationStrategy->ExtraSpace(os);
        indentationStrategy->LineBreak(os);
        for (size_t idx = 0; idx < items.size(); ++idx) {
            indentationStrategy->MakeIndent(os, height + 1);
            os << '"' << items[idx].first << "\": ";
            items[idx].second->Write(os);
            if (idx + 1 < items.size()) {
                os << ',';
                indentationStrategy->ExtraSpace(os);
            }
            indentationStrategy->LineBreak(os);
        }
        indentationStrategy->MakeIndent(os, height);
        if (!Empty()) indentationStrategy->ExtraSpace(os);
        os << "}";
    }

    void IncreaseHeight() override {
        ++height;
        for (const auto& item : items) {
            item.second->IncreaseHeight();
        }
    }

    void SetIndentationStrategy(std::shared_ptr<IndentationStrategy> strategy) override {
        ValueComponent::SetIndentationStrategy(strategy);
        for (const auto& item : items) {
            item.second->SetIndentationStrategy(strategy);
        }
    }

private:
    [[nodiscard]] bool Empty() const { return items.size() == 0; }

private:
    std::vector<KeyValuePair> items;
};

class Int : public ValueComponent
{
public:
    explicit Int(const int value) : value { value } {}

    void Write(std::ostream& os) const override {
        os << value;
    }

private:
    int value;
};

class Float : public ValueComponent
{
public:
    explicit Float(const float value) : value { value } {}

    void Write(std::ostream& os) const override {
        os << value;
    }

private:
    float value;
};

class Bool : public ValueComponent
{
public:
    explicit Bool(const bool value) : value { value } {}

    void Write(std::ostream& os) const override {
        os << std::boolalpha << value;
    }

private:
    bool value;
};

class String : public ValueComponent
{
public:
    explicit String(const std::string& value) : value { value } {}

    void Write(std::ostream& os) const override {
        os << '"' << value << '"';
    }

private:
    std::string value;
};

template<typename T>
class Array : public ValueComponent
{
public:
    void Add(std::unique_ptr<T> item) {
        item->IncreaseHeight();
        item->SetIndentationStrategy(indentationStrategy);
        items.push_back(std::move(item));
    }

    void Write(std::ostream& os) const override {
        if (!indentationStrategy) throw std::runtime_error{ "Indentation strategy is not set!" };

        os << '[';
        if (!Empty()) indentationStrategy->ExtraSpace(os);
        indentationStrategy->LineBreak(os);
        for (size_t idx = 0; idx < items.size(); ++idx) {
            indentationStrategy->MakeIndent(os, height + 1);
            items[idx]->Write(os); 
            if (idx + 1 < items.size()) {
                os << ',';
                indentationStrategy->ExtraSpace(os);
            }
            indentationStrategy->LineBreak(os);
        }
        indentationStrategy->MakeIndent(os, height);
        if (!Empty()) indentationStrategy->ExtraSpace(os);
        os << ']';
    }

    void IncreaseHeight() override {
        ++height;
        for (const auto& item : items) {
            item->IncreaseHeight();
        }
    }

private:
    [[nodiscard]] bool Empty() const { return items.size() == 0; }

private:
    std::vector<std::unique_ptr<T>> items;
};

int main()
{
    auto indentationStrategy = std::make_shared<NoIndentStrategy>();
    
    auto root = std::make_unique<Object>();
    root->SetIndentationStrategy(indentationStrategy);
    root->Add("color", std::make_unique<String>("red"));
    root->Add("isHoled", std::make_unique<Bool>(false));

    auto nestedObject = std::make_unique<Object>();
    nestedObject->Add("count", std::make_unique<Int>(4));
    nestedObject->Add("width", std::make_unique<Float>(3.2f));
    auto array = std::make_unique<Array<Int>>();
     array->Add(std::make_unique<Int>(4));
     array->Add(std::make_unique<Int>(8));
    nestedObject->Add("ids", std::move(array));
    root->Add("shapes", std::move(nestedObject));

    root->Write(std::cout);

    auto otherRoot = std::make_unique<Array<Object>>();
    otherRoot->SetIndentationStrategy(indentationStrategy);

    auto object1 = std::make_unique<Object>();
    object1->Add("key1", std::make_unique<String>("value1"));
    auto object2 = std::make_unique<Object>();
    object2->Add("key2", std::make_unique<String>("value2"));


    otherRoot->Add(std::move(object1));
    otherRoot->Add(std::move(object2));

    otherRoot->Write(std::cout);

    auto emptyArray = std::make_unique<Array<String>>();
    emptyArray->SetIndentationStrategy(indentationStrategy);
    emptyArray->Write(std::cout);
}
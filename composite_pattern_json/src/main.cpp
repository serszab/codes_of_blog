#include <iostream>
#include <memory>
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

    void LineBreak(std::ostream& os) const override { os << std::endl;}
    void ExtraSpace(std::ostream& os) const override {}
    void MakeIndent(std::ostream& os, const size_t height) const override { os << std::string(height * indent, ' '); }

private:
    const int indent;
};

class ValueComponent
{
public:
    virtual ~ValueComponent() = default;

    void SetIndentationStrategy(std::shared_ptr<IndentationStrategy> strategy) {
        indentationStrategy = strategy;
    }

    void SetHeight(const int height) {
        this->height = height;
    }

    virtual void Write(std::ostream& os) const = 0;

protected:
    std::shared_ptr<IndentationStrategy> indentationStrategy{ nullptr };
    int height{ 0 };
};

class Object : public ValueComponent
{
private:
    using KeyValuePair = std::pair<std::string, std::unique_ptr<ValueComponent>>;

public:
    void Add(const std::string& key, std::unique_ptr<ValueComponent> value) {
        value->SetIndentationStrategy(indentationStrategy);
        value->SetHeight(height + 1);
        items.emplace_back(key, std::move(value));
    }

    void Write(std::ostream& os) const override {
        os << '{';
        indentationStrategy->ExtraSpace(os);
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
        indentationStrategy->ExtraSpace(os);
        os << "}";
    }

private:
    std::vector<KeyValuePair> items;
};

class Int : public ValueComponent
{
public:
    explicit Int(const int value) : value{ value } {}

    void Write(std::ostream& os) const override {
        os << value;
    }

private:
    int value;
};

class Float : public ValueComponent
{
public:
    explicit Float(const float value) : value{ value } {}

    void Write(std::ostream& os) const override {
        os << value;
    }

private:
    float value;
};

class Bool : public ValueComponent
{
public:
    explicit Bool(const bool value) : value{ value } {}

    void Write(std::ostream& os) const override {
        os << std::boolalpha << value;
    }

private:
    bool value;
};

class String : public ValueComponent
{
public:
    explicit String(const std::string& value) : value{ value } {}

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
        item->SetIndentationStrategy(indentationStrategy);
        item->SetHeight(height + 1);
        items.push_back(std::move(item));
    }

    void Write(std::ostream& os) const override {
         os << '[';
         indentationStrategy->ExtraSpace(os);
    //     indentationStrategy->LineBreak(os);
    //     for (size_t idx = 0; idx < items.size(); ++idx) {
    //         indentationStrategy->MakeIndent(os, height);
    //         items[idx]->Write(os); 
    //         if (idx + 1 < items.size()) {
    //             os << ',';
    //             indentationStrategy->ExtraSpace(os);
    //         }
    //         indentationStrategy->LineBreak(os);
    //     }
    //     indentationStrategy->MakeIndent(os, height);
    //     indentationStrategy->ExtraSpace(os);
         os << ']';
     }

private:
    std::vector<std::unique_ptr<T>> items;
};

int main()
{
    auto root = std::make_unique<Object>();
    root->SetIndentationStrategy(std::make_shared<IndentStrategy>(4));
    root->Add("color", std::make_unique<String>("red"));
    root->Add("isHoled", std::make_unique<Bool>(false));

    auto nestedObject = std::make_unique<Object>();
    nestedObject->Add("count", std::make_unique<Int>(4));
    nestedObject->Add("width", std::make_unique<Float>(3.2));
    auto array = std::make_unique<Array<Int>>();
    // array->Add(std::make_unique<Int>(4));
    // array->Add(std::make_unique<Int>(8));
    nestedObject->Add("ids", std::move(array));
    root->Add("shapes", std::move(nestedObject));
    
    root->Write(std::cout);
}
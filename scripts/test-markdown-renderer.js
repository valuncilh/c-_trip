const assert = require("node:assert/strict");
const fs = require("node:fs");
const path = require("node:path");

const html = fs.readFileSync(
  path.join(__dirname, "..", "index.html"),
  "utf8",
);

const start = html.indexOf("const escapeHtml = s =>");
const end = html.indexOf("\nfunction loadScriptOnce", start);
assert.ok(start >= 0 && end > start, "Markdown renderer functions were not found");

const rendererFactory = new Function(
  html.slice(start, end) +
    "\nreturn { escapeHtml, inlineMd, renderTextBlocks, richMd };",
);
const { richMd } = rendererFactory();

const imageHtml = richMd("![<sensor>](https://example.com/sensor.png)");
assert.match(imageHtml, /<img src="https:\/\/example\.com\/sensor\.png"/);
assert.match(imageHtml, /alt="&lt;sensor&gt;"/);
assert.match(imageHtml, /loading="lazy"/);
assert.match(imageHtml, /image-caption/);

const relativeImage = richMd("![plot](./content/images/plot.png)");
assert.match(relativeImage, /src="\.\/content\/images\/plot\.png"/);

const javascriptImage = richMd("![unsafe](javascript:alert(1))");
assert.doesNotMatch(javascriptImage, /<img\b/i);

const rootPathImage = richMd("![wrong base](/content/images/plot.png)");
assert.doesNotMatch(rootPathImage, /<img\b/i);

const tableHtml = richMd(
  "| Input | Output |\n| --- | --- |\n| 7 / 2 | 3 |",
);
assert.match(tableHtml, /class="table-scroll"/);
assert.match(tableHtml, /<table>/);

const mathHtml = richMd("$$\nx^2 + 1\n$$");
assert.match(mathHtml, /class="math-block"/);

const codeFence = String.fromCharCode(96, 96, 96);
const codeHtml = richMd(codeFence + "cpp\nint main() { return 0; }\n" + codeFence);
assert.match(codeHtml, /<pre><code class="language-cpp">/);
assert.doesNotMatch(codeHtml, /<script>/i);

const untrustedHtml = richMd("<script>alert(1)</script>");
assert.doesNotMatch(untrustedHtml, /<script>/i);
assert.match(untrustedHtml, /&lt;script&gt;/);

console.log("Markdown renderer smoke tests passed: images, links, tables, math, code, and escaping.");
